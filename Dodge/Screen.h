#pragma once

#include <Windows.h>

#include "WorldObject.h"

typedef struct TScreen
{
	int width;
	int height;

	HANDLE screenHandles[2];

	int screenIndex;

	unsigned short primaryForegroundAndBackgroundColor;
	unsigned short latestForegroundAndBackgroundColor;

	wchar_t** textBuffer; // [HEIGHT][WIDTH]
	unsigned short** colorBuffer;
	wchar_t* clearLineText;
	unsigned short* clearLineColor;
} Screen;


Screen* Screen_Create(int _width, int _height, wchar_t* _fontFaceName, COORD _fontSize, unsigned short _foregroundColor, unsigned short _backgroundColor);

void Screen_Release(Screen* _screen);

HANDLE _Screen_CreateScreenHandle(wchar_t* _fontFaceName, COORD _fontSize);

void _Screen_ReleaseScreenHandle(HANDLE _handle);

void Screen_Resize(HANDLE _handle, int _width, int _height);

void Screen_SetHandlePrimaryColor(Screen* _screen, HANDLE _handle, unsigned short _color);

void Screen_SetHandleTextColor(Screen* _screen, HANDLE _handle, unsigned short _color);

wchar_t** _Screen_CreateTextBuffer(int _width, int _height);

void _Screen_ReleaseTextBuffer(int _size, wchar_t** _buffer);

unsigned short** _Screen_CreateColorBuffer(int _width, int _height);

void _Screen_ReleaseColorBuffer(int _height, unsigned short** _buffer);

void Screen_Render(Screen* _screen);

void Screen_Print(Screen* _screen, int _startX, int _startY, wchar_t** _image, int _imageWidth, int _imageHeight);

void Screen_PrintLine(Screen* _screen, int _startX, int _startY, wchar_t* _line, int _lineWidth);

void Screen_PrintWithColor(Screen* _screen, int _startX, int _startY, wchar_t** _image, int _imageWidth, int _imageHeight, unsigned short _color);

void Screen_PrintLineWithColor(Screen* _screen, int _startX, int _startY, wchar_t* _line, int _lineWidth, unsigned short _color);

void Screen_ClearScreen(Screen* _screen);

void Screen_ClearBuffer(Screen* _screen);

void _Screen_Print(Screen* _screen, int _startX, int _startY, wchar_t** _image, int _imageWidth, int _imageHeight, unsigned short _color);

void _Screen_PrintLine(Screen* _screen, int _startX, int _startY, wchar_t* _line, int _lineWidth, unsigned short _color);

void _Screen_WriteLineToConsole(Screen* _screen, int _startY, wchar_t* _textBufferLine, unsigned short* _colorBufferLine);

void Screen_PrintSprite(Screen* _screen, int _startX, int _startY, Sprite* _sprite);

void Screen_PrintWorldObject(Screen* _screen, WorldObject* _worldObject);

void _Screen_PrintSpriteWithMask(Screen* _screen, int _startX, int _startY, Sprite* _sprite);
