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

void Screen_Print(Screen* _screen, int _startX, int _startY, wchar_t** _image, int _imageWidth, int _imageHeight)
{
	if (_screen == NULL || _image == NULL)
		return;

	if (_imageWidth <= 0 || _imageHeight <= 0)
		return;

	// 화면의 영역 바깥의 오브젝트는 무시합니다.
	if (_startX >= _screen->width - 1
		|| _startY >= _screen->height - 1
		|| _startX + _imageWidth <= 0
		|| _startY + _imageHeight <= 0)
		return;

	// 출력될 이미지의 영역이 화면 크기를 넘어 바깥으로 나가 보이지 않는 길이를 구합니다.
	// X축에서 화면 왼쪽으로 넘어가거나 Y축에서 화면 아래쪽으로 넘어가는 이미지인 경우,
	// 이미지가 적절히 보여지기 위해 이미지의 어느 index부터 표현해야 할 것인지 계산합니다.
	int _exceedScreenXCharacterCount = 0;
	int _exceedScreenYLineCount = 0;
	int _imageStartY = 0;
	int _imageStartX = 0;

	if (_startX + _imageWidth >= _screen->width)
	{
		_exceedScreenXCharacterCount = (_startX + _imageWidth) - _screen->width;
	}
	else if (_startX < 0)
	{
		// 화면 왼쪽으로 나간 경우,
		// 화면의 왼쪽 시작 지점부터 출력합니다.
		_exceedScreenXCharacterCount = -_startX;
		_startX = 0;
		_imageStartX = _exceedScreenXCharacterCount;
	}

	if (_startY + _imageHeight > _screen->height)
	{
		_exceedScreenYLineCount = (_startY + _imageHeight) - _screen->height;
	}
	else if (_startY < 0)
	{
		// 화면 아래쪽으로 나간 경우,
		// 화면의 아래쪽 시작 지점부터 출력합니다.
		_exceedScreenYLineCount = -_startY;
		_startY = 0;
		_imageStartY = _exceedScreenYLineCount;
	}

	// 출력할 이미지의 세로 길이를 구합니다.
	const int _lineCount = _imageHeight - _exceedScreenYLineCount;

	// 출력할 이미지의 가로 길이를 구합니다.
	const int _characterCountInLine = _imageWidth - _exceedScreenXCharacterCount;
	const int _widthByteSize = _characterCountInLine * sizeof(wchar_t);

	// 실제로 쓰여질 버퍼의 y 위치를 구합니다.
	const int _bufferStartYIndex = (_screen->height - 1) - _startY;
	const int _bufferEndYIndex = _bufferStartYIndex - _lineCount;

	// 버퍼에 이미지의 값을 덮어씁니다.
	int _bufferY = _bufferStartYIndex;
	int _imageY = _imageStartY;

	for (; _bufferY > _bufferEndYIndex;
	       --_bufferY, ++_imageY)
	{
		const wchar_t* _destination = _screen->textBuffer[_bufferY] + _startX;
		const int _destinationLeftoverByteSize = _msize(_screen->textBuffer[_bufferY]) - _startX * sizeof(wchar_t);
		const wchar_t* _source = _image[_imageY] + _imageStartX;

		memcpy_s(_destination, _destinationLeftoverByteSize, _source, _widthByteSize);
	}
}

void Screen_PrintLine(Screen* _screen, int _startX, int _startY, wchar_t* _line, int _lineWidth)
{
	Screen_Print(_screen, _startX, _startY, &_line, _lineWidth, 1);
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
