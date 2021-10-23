#include "GameInstance.h"

#include <stdio.h>
#include <time.h>

GameInstance* GameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _desiredFps)
{
	GameInstance* _gameInstance = (GameInstance*)calloc(1, sizeof(GameInstance));

	_gameInstance->desiredSleepTimeMillis = 1000 / _desiredFps;

	_gameInstance->startGameMillis = clock();
	_gameInstance->currentPreTickMillis = _gameInstance->startGameMillis;

	_gameInstance->screen = Screen_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize);

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

	Sleep(_gameInstance->desiredSleepTimeMillis);
	_gameInstance->lastPreTickMillis = _gameInstance->currentPreTickMillis;
}
