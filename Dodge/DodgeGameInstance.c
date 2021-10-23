#include "DodgeGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

#include "Sprite.h"

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _boardWidth, int _boardHeight, int _desiredFps)
{
	DodgeGameInstance* _instance = (DodgeGameInstance*)calloc(1, sizeof(DodgeGameInstance));
	_instance->screenWidth = _screenWidth;
	_instance->screenHeight = _screenHeight;

	_instance->gameInstance = GameInstance_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _desiredFps);

	_instance->board = Board_Create(_boardWidth, _boardHeight);

	// for test
	Vector2D _pivot = {0, 0};
	Vector2D _position = {100, 100};
	_instance->player = Player_Create(L"Sprites/test_chihaya.txt", _pivot, _position, 50);

	Vector2D _paimonPivot = {0, 0};
	Vector2D _paimonPosition = {-20, -20};
	_instance->paimon = WorldObject_CreateWithTextFile(L"Sprites/test_paimon.txt", _paimonPivot, _paimonPosition);

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

	Player_Move(_dodgeGame->player, Vector2D_Right, _deltaTime);

	// for test
	if (_dodgeGame->player != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->player->worldObject);

	if (_dodgeGame->paimon != NULL)
		Screen_PrintWorldObject(_screen, _dodgeGame->paimon);
}
