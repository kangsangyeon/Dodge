#include "DodgeGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

#include "Collider.h"
#include "Sprite.h"

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps)
{
	DodgeGameInstance* _instance = (DodgeGameInstance*)calloc(1, sizeof(DodgeGameInstance));
	_instance->screenWidth = _screenWidth;
	_instance->screenHeight = _screenHeight;

	_instance->gameInstance = GameInstance_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _foregroundColor, _backgroundColor, _useColor, _desiredFps);

	_instance->board = Board_Create(_boardWidth, _boardHeight);

	// for test
	Vector2D _pivot = {0, 0};
	Vector2D _position = {100, 100};
	_instance->player = Player_Create(L"Sprites/test_chihaya.txt", L"Sprites/test_chihaya_mask_cutted.txt", _pivot, _position, 50);

	Vector2D _paimonPivot = {0, 0};
	Vector2D _paimonPosition = {-20, -20};
	_instance->paimon = WorldObject_CreateWithSprite(L"Sprites/test_paimon.txt", _paimonPivot, _paimonPosition);

	Vector2D _takanePivot = {0, 0};
	Vector2D _takanePosition = {100, 0};
	_instance->takane = WorldObject_CreateWithSprite(L"Sprites/test_takane.txt", _takanePivot, _takanePosition);

	Vector2D _milizePivot = {0, 0};
	Vector2D _milizePosition = {-20, 30};
	_instance->milize = WorldObject_CreateWithSpriteMask(L"Sprites/test_milize.txt", L"Sprites/test_milize_mask.txt", _milizePivot, _milizePosition);

	_instance->collider1 = Collider_LoadFromTextFile(L"Sprites/test_matrix20_20.txt");
	_instance->collider2 = Collider_LoadFromTextFile(L"Sprites/test_matrix_grid_20_20.txt");

	_instance->_audioClip1 = AudioClip_LoadFromFile(_instance->gameInstance->audio, L"Sounds/test_bgm_yokai_disco.mp3", false);

	if (_instance->_audioClip1 != NULL && Audio_IsPlaying(_instance->_audioClip1) == false)
		Audio_Play(_instance->gameInstance->audio, _instance->_audioClip1, false);


	// 총알 생성
	Vector2D _directionalBulletPivot = {0, 0};


	// 랜덤으로 생성한 위치를 받아와서 if문으로 ditrectional 처리
	Vector2D _directional = { 0,0 };
	// Vector2D _randomPosition = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_randomMiddleHorizontalLine);
	Vector2D _randomPosition = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_directional);
	_instance->directionalBullet = DirectionalBullet_Create(L"Sprites/test_bullet.txt", _directionalBulletPivot, _randomPosition, _directional, 50);

	return _instance;
}

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->board != NULL)
		Board_Release(&_dodgeGame->board);

	if (_dodgeGame->gameInstance != NULL)
		GameInstance_Release(_dodgeGame->gameInstance);

	// for test
	if (_dodgeGame->player != NULL)
		Player_Release(_dodgeGame->player);

	if (_dodgeGame->paimon != NULL)
		WorldObject_Release(_dodgeGame->paimon);

	if (_dodgeGame->takane != NULL)
		WorldObject_Release(_dodgeGame->takane);

	if (_dodgeGame->milize != NULL)
		WorldObject_Release(_dodgeGame->milize);

	if (_dodgeGame->collider1 != NULL)
		Collider_Release(_dodgeGame->collider1);

	if (_dodgeGame->collider2 != NULL)
		Collider_Release(_dodgeGame->collider2);

	if (_dodgeGame->_audioClip1 != NULL)
		AudioClip_Release(_dodgeGame->_audioClip1);

	if (_dodgeGame->_audioClip2 != NULL)
		AudioClip_Release(_dodgeGame->_audioClip2);

	if (_dodgeGame->directionalBullet != NULL)
		WorldObject_Release(_dodgeGame->directionalBullet);

	free(_dodgeGame);
}

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	float _deltaTime = -1;

	GameInstance_PreTick(_dodgeGame->gameInstance, &_deltaTime);

	_DodgeGameInstance_GameTick(_dodgeGame, _deltaTime);

	GameInstance_PostTick(_dodgeGame->gameInstance);
}

void _DodgeGameInstance_GameTick(DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	const Screen* _screen = _dodgeGame->gameInstance->screen;
	const Board* _board = _dodgeGame->board;

	const int _boardWidth = _board->width;
	const int _boardHeight = _board->height;

	wchar_t _text[200] = {0};

	int _screenY = _screen->height - 1;

	Vector2D _velocity = {0, 0};

	if (GetAsyncKeyState(VK_RIGHT))
		_velocity.x += 1;

	if (GetAsyncKeyState(VK_LEFT))
		_velocity.x -= 1;

	if (GetAsyncKeyState(VK_UP))
		_velocity.y += 1;

	if (GetAsyncKeyState(VK_DOWN))
		_velocity.y -= 1;

	_velocity = Vector2D_Normalized(_velocity);

	Player_Move(_dodgeGame->player, _velocity, _deltaTime);

	// for test
	if (_dodgeGame->paimon != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->paimon);

	if (_dodgeGame->takane != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->takane);

	if (_dodgeGame->milize != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->milize);

	if (_dodgeGame->player != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->player->worldObject);

	if (_dodgeGame->collider1 != NULL && _dodgeGame->collider2 != NULL)
	{
		Vector2D _collider1Position = {0, 0};
		Vector2D _collider2Position = {15, 15};

		CollisionInfo* _collisionInfo = NULL;

		Collider_CheckCollisionDebug(_dodgeGame->collider1, _collider1Position, _dodgeGame->collider2, _collider2Position, &_collisionInfo);
	}

	if (_dodgeGame->_audioClip1 != NULL && GameInstance_GetGameTime(_dodgeGame->gameInstance) > 5.f)
	{
		Audio_Stop(_dodgeGame->_audioClip1);
		_dodgeGame->_audioClip1 = NULL;
	}

	// 총알 이동 + 그려주기 + 몇 초마다 생성할 것인지를 결정한다.
	// test
	// DirectionalBullet_RandomPositionCreate(_screen->width, _screen->height);

	DirectionalBullet_Move(_dodgeGame->directionalBullet, _deltaTime);

	if (_dodgeGame->directionalBullet != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->directionalBullet->worldObject);
}
