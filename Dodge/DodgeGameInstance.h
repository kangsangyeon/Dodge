#pragma once
#include <stdbool.h>
#include <wchar.h>

#include "Board.h"
#include "GameInstance.h"

typedef struct TDodgeGameInstance
{
	GameInstance* gameInstance;
	Board* board;

	int screenWidth;
	int screenHeight;

	// for test
	Sprite* testSprite;
} DodgeGameInstance;

DodgeGameInstance* createDodgeGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _boardWidth, int _boardHeight, int _desiredFps);

void releaseDodgeGameInstance(DodgeGameInstance* _dodgeGame);

void tick(DodgeGameInstance* _dodgeGame);

void gameTick(DodgeGameInstance* _dodgeGame, float _deltaTime);
