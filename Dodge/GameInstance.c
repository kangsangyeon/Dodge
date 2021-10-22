#include "GameInstance.h"

#include <stdio.h>
#include <time.h>

GameInstance* createGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _desiredFps)
{
	GameInstance* _gameInstance = (GameInstance*)calloc(1, sizeof(GameInstance));

	_gameInstance->desiredSleepTimeMillis = 1000 / _desiredFps;

	_gameInstance->startGameMillis = clock();
	_gameInstance->currentPreTickMillis = _gameInstance->startGameMillis;

	_gameInstance->screen = createScreen(_screenWidth, _screenHeight, _fontFaceName, _fontSize);

	return _gameInstance;
}

void releaseGameInstance(GameInstance* _gameInstance)
{
	if (_gameInstance == NULL)
		return;

	if (_gameInstance->screen != NULL)
		releaseScreen(_gameInstance->screen);

	free(_gameInstance);
}

void preTick(GameInstance* _gameInstance, float* _outDeltaTime)
{
	_gameInstance->currentPreTickMillis = clock();
	const float _deltaTime = ((double)_gameInstance->currentPreTickMillis - (double)_gameInstance->lastPreTickMillis) / CLOCKS_PER_SEC;

	if (_outDeltaTime != NULL)
		*_outDeltaTime = _deltaTime;
}

void postTick(GameInstance* _gameInstance)
{
	render(_gameInstance->screen);

	Sleep(_gameInstance->desiredSleepTimeMillis);
	_gameInstance->lastPreTickMillis = _gameInstance->currentPreTickMillis;
}
