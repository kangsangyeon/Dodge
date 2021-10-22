#pragma once

#include <time.h>

#include "Screen.h"

typedef struct TGameInstance
{
	Screen* screen;

	unsigned int desiredSleepTimeMillis;
	clock_t startGameMillis;
	clock_t lastPreTickMillis;
	clock_t currentPreTickMillis;
} GameInstance;

GameInstance* createGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _desiredFps);

void releaseGameInstance(GameInstance* _gameInstance);

void preTick(GameInstance* _gameInstance, float* _outDeltaTime);

void postTick(GameInstance* _gameInstance);
