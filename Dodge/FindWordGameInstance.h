#pragma once
#include <stdbool.h>
#include <wchar.h>

#include "Board.h"
#include "GameInstance.h"

typedef struct TFindWordGameInstance
{
	GameInstance* gameInstance;
	Board* board;

	int screenWidth;
	int screenHeight;
} FindWordGameInstance;

FindWordGameInstance* createFindWordGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _boardWidth, int _boardHeight, int _desiredFps);

void releaseFindWordGameInstance(FindWordGameInstance* _findWordGame);

void tick(FindWordGameInstance* _findWordGame);

void gameTick(FindWordGameInstance* _findWordGame, float _deltaTime);

void randomizeBoard(FindWordGameInstance* _findWordGame);

bool checkWordRecursive(FindWordGameInstance* _findWordGame, wchar_t* _word, size_t _wordSize, int _x, int _y, int _depth, int _directionX, int _directionY);
