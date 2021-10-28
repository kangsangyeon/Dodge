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
	_instance->player = Player_Create(L"Sprites/player_heart.txt", L"Sprites/player_heart.txt", Vector2D_Center, _screenCenter, 50);

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

	if (_dodgeGame->player != NULL)
	{
		Player_Move(_dodgeGame->player, _velocity, _deltaTime);
		Screen_PrintWorldObject(_screen, _dodgeGame->player->worldObject);
	}
}
