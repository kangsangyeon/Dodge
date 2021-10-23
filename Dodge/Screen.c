#include "Screen.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "Helper.h"

Screen* Screen_Create(int _width, int _height, wchar_t* _fontFaceName, COORD _fontSize)
{
	Screen* outScreen = (Screen*)calloc(1, sizeof(Screen));
	outScreen->width = _width;
	outScreen->height = _height;
	outScreen->screenIndex = 0;

	outScreen->screenHandles[0] = _Screen_CreateScreenHandle(_fontFaceName, _fontSize);
	outScreen->screenHandles[1] = _Screen_CreateScreenHandle(_fontFaceName, _fontSize);

	outScreen->textBuffer = _Screen_CreateTextBuffer(_width, _height);
	Screen_ClearBuffer(outScreen);

	outScreen->clearLine = (wchar_t*)calloc(_width + 1, sizeof(wchar_t));
	for (int i = 0; i < _width; ++i)
		outScreen->clearLine[i] = L' ';

	// 화면의 최대 크기는 최초 렌더링 이후에 갱신되기 때문에,
	// 반드시 1회 렌더링 이후에 크기를 설정해주어야 합니다.
	Screen_ClearScreen(outScreen);
	Screen_Render(outScreen);
	Screen_ClearScreen(outScreen);
	Screen_Render(outScreen);

	Screen_Resize(outScreen->screenHandles[0], _width, _height);
	Screen_Resize(outScreen->screenHandles[1], _width, _height);

	return outScreen;
}

void Screen_Release(Screen* _screen)
{
	if (_screen == NULL)
		return;

	_Screen_ReleaseScreenHandle(_screen->screenHandles[0]);

	_Screen_ReleaseScreenHandle(_screen->screenHandles[1]);

	_Screen_ReleaseTextBuffer(_screen->height, _screen->textBuffer);

	if (_screen->clearLine != NULL)
		free(_screen->clearLine);

	free(_screen);
}

HANDLE _Screen_CreateScreenHandle(wchar_t* _fontFaceName, COORD _fontSize)
{
	const HANDLE handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	BOOL _success = FALSE;

	// SetConsoleCursorInfo
	const CONSOLE_CURSOR_INFO _cursorInfo = {2, false};
	const CONSOLE_FONT_INFOEX _fontInfo = {sizeof _fontInfo, 0, _fontSize, FF_DONTCARE, FW_NORMAL, _fontFaceName};

	_success = SetConsoleCursorInfo(handle, &_cursorInfo);
	_success = SetCurrentConsoleFontEx(handle, FALSE, &_fontInfo);

	// debug me!
	COORD _largestSize = GetLargestConsoleWindowSize(handle);

	return handle;
}

void _Screen_ReleaseScreenHandle(HANDLE _handle)
{
	if (_handle == NULL)
		return;

	CloseHandle(_handle);
}

void Screen_Resize(HANDLE _handle, int _width, int _height)
{
	const COORD newSize = {_width, _height};
	const SMALL_RECT rect = {0, 0, _width - 1, _height - 1};

	BOOL _success = FALSE;

	// SetConsoleScreenBufferInfoEx
	CONSOLE_SCREEN_BUFFER_INFOEX consoleSize = {sizeof(consoleSize)};
	GetConsoleScreenBufferInfoEx(_handle, &consoleSize);
	consoleSize.dwSize = newSize;

	_success = SetConsoleScreenBufferInfoEx(_handle, &consoleSize);

	// SetConsoleWindowInfo
	_success = SetConsoleWindowInfo(_handle, TRUE, &rect);

	// debug me!
	COORD _largestSize = GetLargestConsoleWindowSize(_handle);
}

wchar_t** _Screen_CreateTextBuffer(int _width, int _height)
{
	wchar_t** _outArray = (wchar_t**)calloc(_height, sizeof(wchar_t*));
	for (int _y = 0; _y < _height; ++_y)
	{
		_outArray[_y] = (wchar_t*)calloc(_width + 1, sizeof(wchar_t));
		for (int _x = 0; _x < _width; ++_x)
			_outArray[_y][_x] = L' ';
	}

	return _outArray;
}

void _Screen_ReleaseTextBuffer(int _size, wchar_t** _buffer)
{
	if (_buffer == NULL)
		return;

	for (int i = 0; i < _size; ++i)
	{
		if (_buffer[i] != NULL)
		{
			free(_buffer[i]);
		}
	}

	free(_buffer);
}

void Screen_Render(Screen* _screen)
{
	// Screen_ClearScreen(_screen);

	for (int y = 0; y < _screen->height; ++y)
	{
		_Screen_WriteLineToConsole(_screen, y, _screen->textBuffer[y]);
	}

	// 활성화된(=보여주고 있는) Screen buffer를 front buffer와 back buffer끼리 맞바꿉니다.
	SetConsoleActiveScreenBuffer(_screen->screenHandles[_screen->screenIndex]);
	_screen->screenIndex = !_screen->screenIndex;
}

void Screen_Print(Screen* _screen, int _startX, int _startY, wchar_t** _buffer, int _bufferWidth, int _bufferHeight)
{
	if (_screen == NULL || _buffer == NULL)
		return;

	if (_bufferWidth <= 0 || _bufferHeight <= 0)
		return;

	if (_startX >= _screen->width - 1
		|| _startY >= _screen->height - 1)
		return;

	const int _startYIndex = _screen->height - _startY - 1;
	const int _endYIndex = _startYIndex - _bufferHeight;

	int _exceedScreenCharacterCount = (_startX + _bufferWidth) - _screen->width;
	if (_exceedScreenCharacterCount <= 0)
		_exceedScreenCharacterCount = 0;

	const int _characterCountInLine = _bufferWidth - _exceedScreenCharacterCount;

	const int _widthByteSize = _characterCountInLine * sizeof(wchar_t);

	int _yIndex = _startYIndex;
	for (int _y = 0;
	     _y < _bufferHeight && _yIndex > _endYIndex;
	     ++_y, ++_yIndex)
	{
		memcpy_s(_screen->textBuffer[_yIndex] + _startX, _widthByteSize, _buffer[_y], _widthByteSize);
	}
}

void Screen_PrintLine(Screen* _screen, int _startX, int _startY, wchar_t* _buffer, int _bufferWidth)
{
	Screen_Print(_screen, _startX, _startY, &_buffer, _bufferWidth, 1);
}

void Screen_ClearScreen(Screen* _screen)
{
	for (int _y = 0; _y < _screen->height; ++_y)
	{
		_Screen_WriteLineToConsole(_screen, _y, _screen->clearLine);
	}
}

void Screen_ClearBuffer(Screen* _screen)
{
	for (int _y = 0; _y < _screen->height; ++_y)
	{
		for (int _x = 0; _x < _screen->width; ++_x)
		{
			_screen->textBuffer[_y][_x] = L' ';
		}
	}
}

void _Screen_WriteLineToConsole(Screen* _screen, int _startY, wchar_t* _buffer)
{
	DWORD _dw;

	COORD _coord;
	_coord.X = 0;
	_coord.Y = _startY;

	const int _bytesToWrite = _screen->width * sizeof(wchar_t);

	SetConsoleCursorPosition(_screen->screenHandles[_screen->screenIndex], _coord);

	// WriteFile(_screen->screenHandles[_screen->screenIndex],
	//           _buffer, _bytesToWrite,
	//           &_dw, NULL);

	WriteConsoleW(_screen->screenHandles[_screen->screenIndex],
	              _buffer, wcslen(_buffer),
	              &_dw, NULL);
}

void Screen_PrintSprite(Screen* _screen, int _startX, int _startY, Sprite* _sprite)
{
	for (int _y = 0; _y < _sprite->height; ++_y)
	{
		const int _yScreenPosition = _startY + _y;
		const wchar_t* _spriteLine = _sprite->dataArr[_y];
		const int _spriteLineWidth = wcslen(_spriteLine);

		Screen_PrintLine(_screen, _startX, _yScreenPosition, _spriteLine, _spriteLineWidth);
	}
}

void Screen_PrintWorldObject(Screen* _screen, WorldObject* _worldObject)
{
	const double _startXWorldPosition = _worldObject->position.x - _worldObject->sprite->width * _worldObject->pivot.x;
	const double _startYWorldPosition = _worldObject->position.y - _worldObject->sprite->height * _worldObject->pivot.y;

	const int _startXScreenPosition = floor(_startXWorldPosition);
	const int _startYScreenPosition = floor(_startYWorldPosition);

	Screen_PrintSprite(_screen, _startXScreenPosition, _startYScreenPosition, _worldObject->sprite);
}
