#pragma once

#include <Windows.h>

typedef struct TScreen
{
	int width;
	int height;

	HANDLE screenHandles[2];

	int screenIndex;

	wchar_t** textBuffer; // [HEIGHT][WIDTH]
	wchar_t* clearLine;
} Screen;


Screen* createScreen(int _width, int _height, wchar_t* _fontFaceName, COORD _fontSize);

void releaseScreen(Screen* _screen);

HANDLE createScreenHandles(wchar_t* _fontFaceName, COORD _fontSize);

void releaseScreenHandles(HANDLE _handle);

void resizeScreen(HANDLE _handle, int _width, int _height);

wchar_t** createTextBuffer(int _width, int _height);

void releaseTextBuffer(int _size, wchar_t** _buffer);

void render(Screen* _screen);

void print(Screen* _screen, int _startX, int _startY, wchar_t** _buffer, int _bufferWidth, int _bufferHeight);

void printLine(Screen* _screen, int _startX, int _startY, wchar_t* _buffer, int _bufferWidth);

void clearScreen(Screen* _screen);

void clearBuffer(Screen* _screen);

void writeLineToConsole(Screen* _screen, int _startY, wchar_t* _buffer);
