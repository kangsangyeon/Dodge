#include "FindWordGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

#include "Sprite.h"

FindWordGameInstance* createFindWordGameInstance(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize, int _boardWidth, int _boardHeight, int _desiredFps)
{
	FindWordGameInstance* _instance = (FindWordGameInstance*)calloc(1, sizeof(FindWordGameInstance));
	_instance->screenWidth = _screenWidth;
	_instance->screenHeight = _screenHeight;

	_instance->gameInstance = createGameInstance(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _desiredFps);

	_instance->board = createBoard(_boardWidth, _boardHeight);

	randomizeBoard(_instance);

	return _instance;
}

void releaseFindWordGameInstance(FindWordGameInstance* _findWordGame)
{
	if (_findWordGame == NULL)
		return;

	if (_findWordGame->board != NULL)
		releaseBoard(&_findWordGame->board);

	if (_findWordGame->gameInstance != NULL)
		releaseGameInstance(_findWordGame->gameInstance);

	free(_findWordGame);
}

void tick(FindWordGameInstance* _findWordGame)
{
	if (_findWordGame == NULL)
		return;

	float _deltaTime = -1;

	preTick(_findWordGame->gameInstance, &_deltaTime);

	gameTick(_findWordGame, _deltaTime);

	postTick(_findWordGame->gameInstance);
}

void gameTick(FindWordGameInstance* _findWordGame, float _deltaTime)
{
	const Screen* _screen = _findWordGame->gameInstance->screen;
	const Board* _board = _findWordGame->board;

	const int _boardWidth = _board->width;
	const int _boardHeight = _board->height;

	wchar_t _text[200] = {0};

	int _screenY = _screen->height - 1;

	// // 화면에 보드를 그립니다.
	// for (int _boardY = _boardHeight - 1; _boardY >= 0; --_boardY)
	// {
	// 	for (int _boardX = 0; _boardX < _boardWidth; ++_boardX)
	// 	{
	// 		const int _screenX = _boardX * 2;
	// 		const Cell _cell = getCell(_board, _boardX, _boardY);
	//
	// 		swprintf_s(_text, _countof(_text), L"%c ", _cell.data);
	//
	// 		printLine(_screen, _screenX, _screenY, _text, wcslen(_text));
	// 	}
	//
	// 	--_screenY;
	// }

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

void randomizeBoard(FindWordGameInstance* _findWordGame)
{
	for (int _y = 0; _y < _findWordGame->board->height; ++_y)
	{
		for (int _x = 0; _x < _findWordGame->board->width; ++_x)
		{
			const int _randomIndex = rand() % 26;
			const wchar_t _randomAlphabet = L'A' + _randomIndex;
			Cell _cell;
			_cell.data = _randomAlphabet;

			writeCell(_findWordGame->board, _x, _y, _cell);
		}
	}
}

bool checkWordRecursive(FindWordGameInstance* _findWordGame, wchar_t* _word, size_t _wordSize, int _x, int _y, int _depth, int _directionX, int _directionY)
{
	if (_depth >= _wordSize)
	{
		return true;
	}

	const int _posX = _x + _directionX * _depth;
	const int _posY = _y + _directionY * _depth;

	// 보드 위에 존재하는 유효한 위치인지 검증합니다.


	// 현재 기대하는 글자가 맞는지 확인합니다.
	// 맞다면 다음 글자가 맞는지 검사하기 위해 depth를 하나 올려 다시 검사합니다.


	// 그렇지 않다면 false를 반환합니다.
	return false;
}
