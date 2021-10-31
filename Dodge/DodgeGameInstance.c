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

	// player
	const Vector2D _screenCenter = {_screenWidth / 2, _screenHeight / 2};
	_instance->player = Player_Create(L"Sprites/player_heart.txt", L"Sprites/player_heart.txt", Vector2D_Center, _screenCenter, 120, .5f, 300, .2f, 1.5);

	// boss
	_instance->bossType = EBT_NONE;
	_instance->dogeMusk = NULL;

	//총알
	Vector2D _directionalBulletPivot = { 0, 0 };
	Vector2D _directionalBulletDirectional = { 0,0 };
	Vector2D _directionalBulletRandomPosition = DirectionalBullet_CreateRandomPosition(_screenWidth, _screenHeight, &_directionalBulletDirectional);
	_instance->directionalBullet = DirectionalBullet_Create(L"Sprites/test_bullet.txt", _directionalBulletPivot, _directionalBulletRandomPosition, _directionalBulletDirectional, 110);

	//직선총알
	Vector2D _linearBulletPivot = { 0, 0 };
	Vector2D _linearBulletDirectional = { 0, 0 };
	Vector2D _linearBulletRandomPosition = LinearBullet_CreatRandomPosition(_screenWidth, _screenHeight, &_linearBulletDirectional);
	_instance->linearBullet = LinearBullet_Create(L"Sprites/test_paimon.txt", _linearBulletPivot, _linearBulletRandomPosition, _linearBulletDirectional, 110);

	return _instance;
}

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->gameInstance != NULL)
		GameInstance_Release(_dodgeGame->gameInstance);

	// player
	if (_dodgeGame->player != NULL)
		Player_Release(_dodgeGame->player);

	// boss
	if (_dodgeGame->dogeMusk != NULL)
		Boss_DogeMusk_Release(_dodgeGame->dogeMusk);

	//총알
	if (_dodgeGame->directionalBullet != NULL)
		WorldObject_Release(_dodgeGame->directionalBullet);

	//직선총알
	if (_dodgeGame->linearBullet != NULL)
		WorldObject_Release(_dodgeGame->linearBullet);

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

	// boss
	if (GameInstance_GetGameTime(_dodgeGame->gameInstance) > 1
		&& _dodgeGame->bossType == EBT_NONE)
	{
		// 보스가 재생될 시간이 되었고, 보스가 재생중이지 않을 때
		// 보스를 새로 재생합니다.

		if (_dodgeGame->dogeMusk == NULL)
		{
			_dodgeGame->bossType = EBT_DOGE_MUSK;
			_dodgeGame->dogeMusk = Boss_DogeMusk_Create(_dodgeGame);
		}
	}

	switch (_dodgeGame->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_Tick(_dodgeGame, _dodgeGame->dogeMusk, _deltaTime);
		break;
	}

	if (_dodgeGame->dogeMusk != NULL)
		Boss_DogeMusk_DrawTick(_dodgeGame, _dodgeGame->dogeMusk);

	// player
	if (_dodgeGame->player != NULL)
	{
		Player_Tick(_dodgeGame->player, _deltaTime, GameInstance_GetGameTime(_dodgeGame->gameInstance), _screen->width, _screen->height);


		if (_dodgeGame->player->flickerAnim->enable == true && _dodgeGame->player->flickerAnim->visible == true)
			Screen_PrintWorldObject(_screen, _dodgeGame->player->worldObject);
		else if (_dodgeGame->player->flickerAnim->enable == false)
			Screen_PrintWorldObject(_screen, _dodgeGame->player->worldObject);
	}

	//총알
	if (_dodgeGame->directionalBullet != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->directionalBullet->worldObject);

	//직선총알
	if (_dodgeGame->linearBullet != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->linearBullet->worldObject);


	const int _destoryWidth = _screen->width;
	const int _destoryHeight = _screen->height;
	DirectionalBullet_Move(_dodgeGame->directionalBullet, _deltaTime);
	LinearBullet_Move(_dodgeGame->linearBullet, _deltaTime);

	bool _checkDestroy = DirectionalBullet_Destroy(_dodgeGame->directionalBullet, _destoryWidth, _destoryHeight);

	if (_checkDestroy == true)
		_dodgeGame->directionalBullet = NULL;

	switch (_dodgeGame->bossType)
	{
	case EBT_DOGE_MUSK:
		Boss_DogeMusk_CollisionTick(_dodgeGame, _dodgeGame->dogeMusk);
		break;
	}
}
