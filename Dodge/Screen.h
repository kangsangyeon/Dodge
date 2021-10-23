#pragma once

#include <Windows.h>

#include "WorldObject.h"

typedef struct TScreen
{
	int width;
	int height;

	HANDLE screenHandles[2];

	int screenIndex;

	wchar_t** textBuffer; // [HEIGHT][WIDTH]
	wchar_t* clearLine;
} Screen;


Screen* Screen_Create(int _width, int _height, wchar_t* _fontFaceName, COORD _fontSize);

void Screen_Release(Screen* _screen);

HANDLE _Screen_CreateScreenHandle(wchar_t* _fontFaceName, COORD _fontSize);

void _Screen_ReleaseScreenHandle(HANDLE _handle);

void Screen_Resize(HANDLE _handle, int _width, int _height);

wchar_t** _Screen_CreateTextBuffer(int _width, int _height);

void _Screen_ReleaseTextBuffer(int _size, wchar_t** _buffer);

void Screen_Render(Screen* _screen);

void Screen_Print(Screen* _screen, int _startX, int _startY, wchar_t** _image, int _imageWidth, int _imageHeight);

void Screen_PrintLine(Screen* _screen, int _startX, int _startY, wchar_t* _line, int _lineWidth);

void Screen_ClearScreen(Screen* _screen);

void Screen_ClearBuffer(Screen* _screen);

void _Screen_WriteLineToConsole(Screen* _screen, int _startY, wchar_t* _buffer);

void Screen_PrintSprite(Screen* _screen, int _startX, int _startY, Sprite* _sprite);

void Screen_PrintWorldObject(Screen* _screen, WorldObject* _worldObject);
