﻿#pragma once

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

GameInstance* GameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _desiredFps);

void GameInstance_Release(GameInstance* _gameInstance);

void GameInstance_PreTick(GameInstance* _gameInstance, float* _outDeltaTime);

void GameInstance_PostTick(GameInstance* _gameInstance);
