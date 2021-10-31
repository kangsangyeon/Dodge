#include "Scene_Game.h"

#include "Boss_DogeMusk.h"
#include "DirectionalBullet.h"
#include "GameInstance.h"
#include "LinearBullet.h"
#include "Player.h"
#include "DodgeGameInstance.h"

Scene_Game* Scene_Game_Create(DodgeGameInstance* _dodgeGame)
{
	const GameInstance* _gameInstance = _dodgeGame->gameInstance;
	const int _screenWidth = _gameInstance->screen->width;
	const int _screenHeight = _gameInstance->screen->height;

	Scene_Game* _outScene = (Scene_Game*)malloc(sizeof(Scene_Game));

	// player
	const Vector2D _screenCenter = {_screenWidth / 2, _screenHeight / 2};
	_outScene->player = Player_Create(L"Sprites/player_heart.txt", L"Sprites/player_heart.txt", Vector2D_Center, _screenCenter, 120, .5f, 300, .2f);

	// boss
	_outScene->bossType = EBT_NONE;
	_outScene->dogeMusk = NULL;

	//총알
	Vector2D _directionalBulletPivot = {0, 0};
	Vector2D _directionalBulletDirectional = {0, 0};
	Vector2D _directionalBulletRandomPosition = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_directionalBulletDirectional);
	_outScene->directionalBullet = DirectionalBullet_Create(L"Sprites/test_bullet.txt", _directionalBulletPivot, _directionalBulletRandomPosition, _directionalBulletDirectional,
	                                                        110);

	//직선총알
	Vector2D _linearBulletPivot = {0, 0};
	Vector2D _linearBulletDirectional = {0, 0};
	Vector2D _linearBulletRandomPosition = LinearBullet_CreatRandomPosition(_screenWidth, _screenHeight, &_linearBulletDirectional);
	_outScene->linearBullet = LinearBullet_Create(L"Sprites/test_paimon.txt", _linearBulletPivot, _linearBulletRandomPosition, _linearBulletDirectional, 110);

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

	//총알
	if (_scene->directionalBullet != NULL)
		DirectionalBullet_Release(_scene->directionalBullet);

	//직선총알
	if (_scene->linearBullet != NULL)
		LinearBullet_Release(_scene->linearBullet);

	free(_scene);
}

void Scene_Game_OnEnter(Scene_Game* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void Scene_Game_OnExit(Scene_Game* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void Scene_Game_Tick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	const GameInstance* _gameInstance = _dodgeGame->gameInstance;
	const Screen* _screen = _gameInstance->screen;

	// boss
	if (GameInstance_GetGameTime(_gameInstance) > 1
		&& _scene->bossType == EBT_NONE)
	{
		// 보스가 재생될 시간이 되었고, 보스가 재생중이지 않을 때
		// 보스를 새로 재생합니다.

		if (_scene->dogeMusk == NULL)
		{
			_scene->bossType = EBT_DOGE_MUSK;
			_scene->dogeMusk = Boss_DogeMusk_Create(_gameInstance);
		}
	}

	switch (_scene->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_Tick(_gameInstance, _scene->dogeMusk, _deltaTime);
		break;
	}

	if (_scene->dogeMusk != NULL)
		Boss_DogeMusk_DrawTick(_gameInstance, _scene->dogeMusk);

	// player
	if (_scene->player != NULL)
	{
		Player_Tick(_scene->player, _deltaTime, GameInstance_GetGameTime(_gameInstance), _screen->width, _screen->height);
		Screen_PrintWorldObject(_screen, _scene->player->worldObject);
	}

	//총알
	if (_scene->directionalBullet != NULL)
		Screen_PrintWorldObject(_screen, _scene->directionalBullet->worldObject);

	//직선총알
	if (_scene->linearBullet != NULL)
		Screen_PrintWorldObject(_screen, _scene->linearBullet->worldObject);


	const int _destoryWidth = _screen->width;
	const int _destoryHeight = _screen->height;
	DirectionalBullet_Move(_scene->directionalBullet, _deltaTime);
	LinearBullet_Move(_scene->linearBullet, _deltaTime);

	bool _checkDestroy = DirectionalBullet_Destroy(_scene->directionalBullet, _destoryWidth, _destoryHeight);

	if (_checkDestroy == true)
		_scene->directionalBullet = NULL;
}
