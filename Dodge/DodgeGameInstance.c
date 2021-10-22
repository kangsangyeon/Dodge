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
	swprintf_s(_text, _countof(_text), L"ABCDEFGHIJKLMNOPQRSTU");
	printLine(_screen, 0, _screenY, _text, wcslen(_text));
	--_screenY;

	swprintf_s(_text, _countof(_text), L"찾을 문자열을 입력해주세요");
	printLine(_screen, 0, _screenY, _text, wcslen(_text));
	--_screenY;

	wchar_t _testText[] = L"찾을 문자열을 입력해주세요";
	int _textLength;
	_textLength = wcslen(_testText);
	printLine(_screen, 0, _screenY, _testText, _textLength);
	--_screenY;

	swprintf_s(_text, _countof(_text), L"찾을 문자열을 입력해주세요");
	printLine(_screen, 0, _screenY, _text, wcslen(_text));
	--_screenY;
}
