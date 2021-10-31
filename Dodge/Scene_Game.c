#include "Scene_Game.h"

#include "Boss_DogeMusk.h"
#include "DirectionalBullet.h"
#include "GameInstance.h"
#include "Player.h"
#include "DodgeGameInstance.h"


const int g_GameStateDurations[10] = {
	// 대기 시간은 3초입니다.
	-1, 0, 3,
	// BOSS 를 제외한 모든 상태는 10초간 유지됩니다.
	10, 10, 10, 10, 10, 10,
	// BOSS는 20초간 출현합니다.
	20
};

const int g_GameStateStartBulletCount[10] = {
	-1, -1, -1,
	// EASY
	10, 1,
	// NORMAL
	10, 1,
	// HARD
	10, 1,
	0
};

const int g_GameStateSpawnBulletDelay[10] = {
	-1, -1, -1,
	// EASY
	1, 2,
	// NORMAL
	1, 2,
	// HARD
	1, 2,
	0
};

const float g_GameStateBulletSpeed[10] = {
	-1, -1, -1,
	// EASY
	50, 50,
	// NORMAL
	70, 60,
	// HARD
	100, 70,
	0
};

const int g_NormalBulletSpriteFilesCount = 2;
const wchar_t* g_NormalBulletSpriteFiles[2] = {L"Sprites/Bullets/bullet_normal_small.txt", L"Sprites/Bullets/bullet_normal_big.txt"};

const int g_CharacterBulletSpriteImageFilesCount = 6;
const wchar_t* g_CharacterBulletSpriteImageFiles[6] = {
	L"Sprites/Bullets/Lbullet_paimon.txt", L"Sprites/Bullets/Lbullet_nico.txt",
	L"Sprites/Bullets/Lbullet_chihaya.txt", L"Sprites/Bullets/Lbullet_frog.txt",
	L"Sprites/Bullets/Lbullet_pikachu.txt", L"Sprites/Bullets/Lbullet_saitama.txt"
};

const wchar_t* g_CharacterBulletSpriteMaskFiles[6] = {
	L"Sprites/Bullets/Lbullet_paimon_mask.txt", L"Sprites/Bullets/Lbullet_nico_mask.txt",
	L"Sprites/Bullets/Lbullet_chihaya_mask.txt", L"Sprites/Bullets/Lbullet_frog_mask.txt",
	L"Sprites/Bullets/Lbullet_pikachu_mask.txt", L"Sprites/Bullets/Lbullet_saitama_mask.txt"
};

Scene_Game* Scene_Game_Create(DodgeGameInstance* _dodgeGame)
{
	const GameInstance* _gameInstance = _dodgeGame->gameInstance;
	const int _screenWidth = _gameInstance->screen->width;
	const int _screenHeight = _gameInstance->screen->height;

	Scene_Game* _outScene = (Scene_Game*)malloc(sizeof(Scene_Game));

	_outScene->enterSceneTime = 0;
	_outScene->gameState = EGS_START;
	_outScene->gameStateStartTime = 0;
	_outScene->lastSpawnBulletTime = 0;
	_outScene->gameBossState = EGBS_NONE;
	_outScene->bossType = EBT_NONE;

	// player
	const Vector2D _screenCenter = {_screenWidth / 2, _screenHeight / 2};
	_outScene->player = Player_Create(_gameInstance, L"Sprites/player_heart.txt", L"Sprites/player_heart.txt", Vector2D_Center, _screenCenter, 120, .5f, 300, .2f, 2.);

	// boss
	_outScene->dogeMusk = NULL;

	// bullets
	const int _bulletArrByteSize = sizeof(_outScene->bulletArr);
	memset(_outScene->bulletArr, 0, _bulletArrByteSize);
	
	_outScene->ingameBgmClip = AudioClip_LoadFromFile(_dodgeGame->gameInstance->audio, L"Sounds/Bgm/ingame_bgm.wav", false);


	return _outScene;
}

void Scene_Game_Release(Scene_Game* _scene)
{
	if (_scene == NULL)
		return;

	// player
	if (_scene->player != NULL)
		Player_Release(_scene->player);

	// boss
	if (_scene->dogeMusk != NULL)
		Boss_DogeMusk_Release(_scene->dogeMusk);

	// bullet
	if (_scene->bulletArr != NULL)
	{
		for (int i = 0; i < 50; ++i)
		{
			DirectionalBullet* _bullet = _scene->bulletArr[i];
			if (_bullet != NULL)
				DirectionalBullet_Release(_bullet);
		}
	}

	if (_scene->ingameBgmClip != NULL)
		AudioClip_Release(_scene->ingameBgmClip);

	free(_scene);
}

void Scene_Game_OnEnter(Scene_Game* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	const float _gameTime = GameInstance_GetGameTime(_dodgeGame->gameInstance);

	_scene->enterSceneTime = _gameTime;
	_scene->gameStateStartTime = _gameTime;
	_scene->gameState = EGS_WAITING;

	// player
	Player_Initialize(_scene->player);

	if (_scene->ingameBgmClip != NULL)
		Audio_Play(_dodgeGame->gameInstance->audio, _scene->ingameBgmClip, true);
}

void Scene_Game_OnExit(Scene_Game* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->ingameBgmClip != NULL)
		Audio_Stop(_scene->ingameBgmClip);
}

void Scene_Game_Tick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	const GameInstance* _gameInstance = _dodgeGame->gameInstance;
	const Screen* _screen = _gameInstance->screen;

	// 게임을 클리어했다면, GameClear 씬으로 전환합니다.
	if (_scene->gameState == EGS_CLEAR)
	{
		DodgeGameInstance_ChangeScene(_dodgeGame, EST_GAMECLAER);
		return;
	}


	// 플레이어는 게임 상태와 상관없이 움직일 수 있습니다.
	if (_scene->player != NULL)
		Player_Tick(_gameInstance, _scene->player, _deltaTime);

	const bool _stateTransion = _Scene_Game_CheckStateTransitionTick(_scene, _dodgeGame, _deltaTime);

	/* 상태에 대응되는 Tick을 실행합니다. */
	switch (_scene->gameState)
	{
	case EGS_START:
		_Scene_Game_StartStateTick(_scene, _dodgeGame, _deltaTime, _stateTransion);
		break;
	case EGS_NORMAL_EASY:
	case EGS_NORMAL_NORMAL:
	case EGS_NORMAL_HARD:
	case EGS_CHARACTER_EASY:
	case EGS_CHARACTER_NORMAL:
	case EGS_CHARACTER_HARD:
		_Scene_Game_NormalStateTick(_scene, _dodgeGame, _deltaTime, _stateTransion);
		break;
	case EGS_BOSS:
		_Scene_Game_BossStateTick(_scene, _dodgeGame, _deltaTime, _stateTransion);
	case EGS_CLEAR:
		_Scene_Game_ClearStateTick(_scene, _dodgeGame, _deltaTime, _stateTransion);
		break;
	}

	// 플레이어는 게임 상태와 상관없이 그립니다.
	if (_scene->player != NULL)
		Player_DrawTick(_gameInstance, _scene->player);
}

bool _Scene_Game_CheckStateTransitionTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return false;

	if (_scene->gameState == EGS_CLEAR)
		return false;

	const EGameState _lastState = _scene->gameState;
	const double _gameTime = GameInstance_GetGameTime(_dodgeGame->gameInstance);

	const double _stateDuration = g_GameStateDurations[_scene->gameState];

	if (_stateDuration >= 0)
	{
		const bool _shouldStartGame = _gameTime - _scene->gameStateStartTime >= _stateDuration;
		if (_shouldStartGame)
			_scene->gameState = _scene->gameState + 1;
	}

	if (_scene->gameState != _lastState)
	{
		// 이번 프레임에서 게임 상태가 전이되었다면,
		// gameStateTime을 현재 시간으로 새로고칩니다.
		_scene->gameStateStartTime = _gameTime;

		// 현재 씬에 존재하는 총알을 모두 제거합니다.
		ReleaseBullets(_scene);

		return true;
	}

	return false;
}

void _Scene_Game_StartStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void _Scene_Game_NormalStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	const double _gameTime = GameInstance_GetGameTime(_dodgeGame->gameInstance);
	const Screen* _screen = _dodgeGame->gameInstance->screen;

	if (_stateStart == true)
	{
		// 상태가 전이된 직후에는 총알을 한번에 생성합니다.
		const float _moveSpeed = g_GameStateBulletSpeed[_scene->gameState];
		const int _count = g_GameStateStartBulletCount[_scene->gameState];

		wchar_t** _bulletSpriteImageFiles = NULL;
		wchar_t** _bulletSpriteMaskFiles = NULL;
		switch (_scene->gameState)
		{
		case EGS_NORMAL_EASY:
		case EGS_NORMAL_NORMAL:
		case EGS_NORMAL_HARD:
			_bulletSpriteImageFiles = g_NormalBulletSpriteFiles;
			_bulletSpriteMaskFiles = g_NormalBulletSpriteFiles;
			break;
		case EGS_CHARACTER_EASY:
		case EGS_CHARACTER_NORMAL:
		case EGS_CHARACTER_HARD:
			_bulletSpriteImageFiles = g_CharacterBulletSpriteImageFiles;
			_bulletSpriteMaskFiles = g_CharacterBulletSpriteMaskFiles;
			break;
		}

		SpawnBullets(_scene, _dodgeGame->gameInstance->screen->width, _dodgeGame->gameInstance->screen->height);

		_scene->lastSpawnBulletTime = _gameTime;
	}
	else
	{
		// 총알이 생성된 이후에는 일정 시간마다 총알을 생성합니다.
		const double _elapsedTimeAfterLastSpawnBullet = _gameTime - _scene->lastSpawnBulletTime;
		const double _delay = g_GameStateSpawnBulletDelay[_scene->gameState];

		if (_elapsedTimeAfterLastSpawnBullet >= _delay)
		{
			_scene->lastSpawnBulletTime = _gameTime;

			// 총알을 생성합니다.
			for (int i = 0; i < 50; ++i)
			{
				if (_scene->bulletArr[i] == NULL)
				{
					_scene->bulletArr[i] = SpawnBullet(_scene, _screen->width, _screen->height);
					break;
				}
			}
		}
	}

	for (int i = 0; i < 50; ++i)
	{
		// 총알을 움직입니다.
		// 만약 총알이 이동해서 화면 바깥으로 나간 경우,
		// 새로운 위치와 방향으로 설정해서 다시 발사합니다.
		if (_scene->bulletArr[i] != NULL)
		{
			DirectionalBullet* _bullet = _scene->bulletArr[i];

			DirectionalBullet_Move(_bullet, _deltaTime);

			if (DirectionalBullet_Destroy(_bullet, _screen->width, _screen->height))
			{
				_bullet = SpawnBullet(_scene, _screen->width, _screen->height);
				_scene->bulletArr[i] = _bullet;
			}

			DirectionalBullet_CollisionTick(_dodgeGame, _bullet, _scene->player);

			Screen_PrintWorldObject(_dodgeGame->gameInstance->screen, _scene->bulletArr[i]->worldObject);
		}
	}
}

void _Scene_Game_BossStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->bossType == EBT_NONE)
	{
		// 보스가 재생될 시간이 되었고, 보스가 재생중이지 않을 때
		// 보스를 새로 재생합니다.

		if (_scene->dogeMusk == NULL)
		{
			_scene->bossType = EBT_DOGE_MUSK;
			_scene->dogeMusk = Boss_DogeMusk_Create(_dodgeGame->gameInstance);
		}
	}

	// Collision Tick
	switch (_scene->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_CollisionTick(_dodgeGame, _scene->dogeMusk);
		break;
	}

	// Tick
	switch (_scene->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_Tick(_dodgeGame, _scene->dogeMusk, _deltaTime);
		break;
	}

	// Draw Tick
	switch (_scene->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_DrawTick(_dodgeGame->gameInstance, _scene->dogeMusk);
		break;
	}
}

void _Scene_Game_ClearStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void ReleaseBullets(Scene_Game* _scene)
{
	if (_scene == NULL)
		return;

	const int _arraySize = sizeof(_scene->bulletArr) / sizeof(DirectionalBullet*);
	for (int i = 0; i < _arraySize; ++i)
	{
		if (_scene->bulletArr[i] != NULL)
		{
			DirectionalBullet_Release(_scene->bulletArr[i]);
			_scene->bulletArr[i] = NULL;
		}
	}
}

void SpawnBullets(Scene_Game* _scene, int _screenWidth, int _screenHeight)
{
	if (_scene == NULL)
		return;

	wchar_t** _bulletSpriteImageFiles = NULL;
	wchar_t** _bulletSpriteMaskFiles = NULL;
	int _filesCount = 0;
	switch (_scene->gameState)
	{
	case EGS_NORMAL_EASY:
	case EGS_NORMAL_NORMAL:
	case EGS_NORMAL_HARD:
		_bulletSpriteImageFiles = g_NormalBulletSpriteFiles;
		_bulletSpriteMaskFiles = g_NormalBulletSpriteFiles;
		_filesCount = g_NormalBulletSpriteFilesCount;
		break;
	case EGS_CHARACTER_EASY:
	case EGS_CHARACTER_NORMAL:
	case EGS_CHARACTER_HARD:
		_bulletSpriteImageFiles = g_CharacterBulletSpriteImageFiles;
		_bulletSpriteMaskFiles = g_CharacterBulletSpriteMaskFiles;
		_filesCount = g_CharacterBulletSpriteImageFilesCount;
		break;
	}

	const int _spawnCount = g_GameStateStartBulletCount[_scene->gameState];
	const float _moveSpeed = g_GameStateBulletSpeed[_scene->gameState];

	const int _arrayCount = _filesCount;

	for (int i = 0; i < _spawnCount; ++i)
	{
		const int _randomIndex = rand() % _arrayCount;

		const wchar_t* _randomImageFile = _bulletSpriteImageFiles[_randomIndex];
		const wchar_t* _randomMaskFile = _bulletSpriteMaskFiles[_randomIndex];

		Vector2D _randomDirection = Vector2D_Zero;
		const Vector2D _randomPosition = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_randomDirection);

		_scene->bulletArr[i] = DirectionalBullet_Create(_randomImageFile, _randomMaskFile, Vector2D_Center, _randomPosition, _randomDirection, _moveSpeed);
	}
}

DirectionalBullet* SpawnBullet(Scene_Game* _scene, int _screenWidth, int _screenHeight)
{
	if (_scene == NULL)
		return NULL;

	wchar_t** _bulletSpriteImageFiles = NULL;
	wchar_t** _bulletSpriteMaskFiles = NULL;
	int _filesCount = 0;
	switch (_scene->gameState)
	{
	case EGS_NORMAL_EASY:
	case EGS_NORMAL_NORMAL:
	case EGS_NORMAL_HARD:
		_bulletSpriteImageFiles = g_NormalBulletSpriteFiles;
		_bulletSpriteMaskFiles = g_NormalBulletSpriteFiles;
		_filesCount = g_NormalBulletSpriteFilesCount;
		break;
	case EGS_CHARACTER_EASY:
	case EGS_CHARACTER_NORMAL:
	case EGS_CHARACTER_HARD:
		_bulletSpriteImageFiles = g_CharacterBulletSpriteImageFiles;
		_bulletSpriteMaskFiles = g_CharacterBulletSpriteMaskFiles;
		_filesCount = g_CharacterBulletSpriteImageFilesCount;
		break;
	}

	const int _arrayCount = _filesCount;
	const int _randomIndex = rand() % _arrayCount;

	const wchar_t* _randomImageFile = _bulletSpriteImageFiles[_randomIndex];
	const wchar_t* _randomMaskFile = _bulletSpriteMaskFiles[_randomIndex];

	const float _moveSpeed = g_GameStateBulletSpeed[_scene->gameState];
	Vector2D _direction = Vector2D_Zero;
	const Vector2D _position = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_direction);

	DirectionalBullet* _newBullet = DirectionalBullet_Create(_randomImageFile, _randomMaskFile,
	                                                         Vector2D_Center, _position, _direction, _moveSpeed);

	return _newBullet;
}
