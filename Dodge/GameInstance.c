#include "GameInstance.h"

#include <stdio.h>
#include <time.h>

GameInstance* GameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, unsigned short _foregroundColor, unsigned short _backgroundColor,
                                  int _desiredFps)
{
	GameInstance* _gameInstance = (GameInstance*)calloc(1, sizeof(GameInstance));

	_gameInstance->desiredSleepTimeMillis = 1000 / _desiredFps;

	_gameInstance->startGameMillis = clock();
	_gameInstance->currentPreTickMillis = _gameInstance->startGameMillis;

	_gameInstance->screen = Screen_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _foregroundColor, _backgroundColor);

	return _gameInstance;
}

void GameInstance_Release(GameInstance* _gameInstance)
{
	if (_gameInstance == NULL)
		return;

	if (_gameInstance->screen != NULL)
		Screen_Release(_gameInstance->screen);

	free(_gameInstance);
}

void GameInstance_PreTick(GameInstance* _gameInstance, float* _outDeltaTime)
{
	Screen_ClearBuffer(_gameInstance->screen);

	_gameInstance->currentPreTickMillis = clock();
	const float _deltaTime = ((double)_gameInstance->currentPreTickMillis - (double)_gameInstance->lastPreTickMillis) / CLOCKS_PER_SEC;

	if (_outDeltaTime != NULL)
		*_outDeltaTime = _deltaTime;
}

void GameInstance_PostTick(GameInstance* _gameInstance)
{
	Screen_Render(_gameInstance->screen);

	const clock_t _processingTimeMillis = clock() - _gameInstance->currentPreTickMillis;
	const clock_t _targetSleepTimeMillis = _gameInstance->desiredSleepTimeMillis - _processingTimeMillis;

	if (_targetSleepTimeMillis > 0)
		Sleep(_targetSleepTimeMillis);

	_gameInstance->lastPreTickMillis = _gameInstance->currentPreTickMillis;
}
