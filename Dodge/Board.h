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

Board* createBoard(int _width, int _height);

void releaseBoard(Board** _board);

void clearBoard(Board* _board);

Cell getCell(Board* _board, int _x, int _y);

Cell* getLine(Board* _board, int _y);

void writeCell(Board* _board, int _x, int _y, Cell _cell);

void writeLine(Board* _board, int _y, Cell* _line);

void clearCell(Board* _board, int _x, int _y);

void clearLine(Board* _board, int _y);
