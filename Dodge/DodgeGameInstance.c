#include "DodgeGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

#include "Sprite.h"

DodgeGameInstance* createDodgeGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _boardWidth, int _boardHeight, int _desiredFps)
{
	DodgeGameInstance* _instance = (DodgeGameInstance*)calloc(1, sizeof(DodgeGameInstance));
	_instance->screenWidth = _screenWidth;
	_instance->screenHeight = _screenHeight;

	_instance->gameInstance = createGameInstance(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _desiredFps);

	_instance->board = createBoard(_boardWidth, _boardHeight);

	return _instance;
}

void releaseDodgeGameInstance(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->board != NULL)
		releaseBoard(&_dodgeGame->board);

	if (_dodgeGame->gameInstance != NULL)
		releaseGameInstance(_dodgeGame->gameInstance);

	free(_dodgeGame);
}

void tick(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	float _deltaTime = -1;

	preTick(_dodgeGame->gameInstance, &_deltaTime);

	gameTick(_dodgeGame, _deltaTime);

	postTick(_dodgeGame->gameInstance);
}

void gameTick(DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	const Screen* _screen = _dodgeGame->gameInstance->screen;
	const Board* _board = _dodgeGame->board;

	const int _boardWidth = _board->width;
	const int _boardHeight = _board->height;

	wchar_t _text[200] = {0};

	int _screenY = _screen->height - 1;

	// 텍스트 입력 안내 메세지를 표시합니다.
	wchar_t* _testText = (wchar_t*)calloc(_dodgeGame->screenWidth + 1, sizeof(wchar_t));
	const int _testTextCount = _msize(_testText) / sizeof(wchar_t);

	for (int i = 0; i < _screen->height; ++i)
	{
		swprintf_s(_testText, _testTextCount, L"%d: 가나다라마바사아자차카타파하가나다라마바사아자차카타파하가나다라마바사아자차카타파하가나다라마바사아자차카타파하", i);

		const int _textLength = wcslen(_testText);
		Screen_PrintLine(_screen, 0, _screenY, _testText, _textLength);
		--_screenY;
	}


	// Screen_PrintLine(_screen, 0, _screenY, _text, wcslen(_text));
	// --_screenY;
}
