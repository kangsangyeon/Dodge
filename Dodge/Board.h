#pragma once
#include <wchar.h>

typedef struct TCell
{
	wchar_t data;
	unsigned char color;
} Cell;

typedef struct TBoard
{
	int width;
	int height;
	Cell** cellArr;
} Board;

Board* Board_Create(int _width, int _height);

void Board_Release(Board** _board);

void Board_Clear(Board* _board);

Cell Board_GetCell(Board* _board, int _x, int _y);

Cell* Board_GetLine(Board* _board, int _y);

void Board_WriteCell(Board* _board, int _x, int _y, Cell _cell);

void Board_WriteLine(Board* _board, int _y, Cell* _line);

void Board_ClearCell(Board* _board, int _x, int _y);

void Board_ClearLine(Board* _board, int _y);
