﻿#include "Board.h"

#include <stdlib.h>

Board* Board_Create(int _width, int _height)
{
	if (_width <= 0 || _height <= 0)
		return;

	Board* _newBoard = (Board*)calloc(1, sizeof(Board));
	_newBoard->width = _width;
	_newBoard->height = _height;

	_newBoard->cellArr = (Cell**)calloc(_height, sizeof(Cell*));
	for (int i = 0; i < _height; ++i)
	{
		_newBoard->cellArr[i] = (Cell*)calloc(_width, sizeof(Cell));
	}

	return _newBoard;
}

void Board_Release(Board** _board)
{
	Board* _actualBoard = *_board;

	if (_actualBoard == NULL)
		return;

	if (_actualBoard->cellArr != NULL)
	{
		for (int i = 0; i < _actualBoard->height; ++i)
		{
			if (_actualBoard->cellArr[i] != NULL)
				free(_actualBoard->cellArr[i]);
		}

		free(_actualBoard->cellArr);
	}

	free(_actualBoard);
}

void Board_Clear(Board* _board)
{
	if (_board == NULL ||
		_board->cellArr == NULL)
		return;

	for (int _y = 0; _y < _board->height; ++_y)
	{
		if (_board->cellArr + _y == NULL)
			continue;

		memset(_board->cellArr + _y, 0, _board->width * sizeof(Cell));
	}
}

Cell Board_GetCell(Board* _board, int _x, int _y)
{
	const int _yIndex = (_board->height - 1) - _y;

	return _board->cellArr[_yIndex][_x];
}

Cell* Board_GetLine(Board* _board, int _y)
{
	const int _yIndex = (_board->height - 1) - _y;

	return _board->cellArr[_yIndex];
}

void Board_WriteCell(Board* _board, int _x, int _y, Cell _cell)
{
	const int _yIndex = (_board->height - 1) - _y;

	_board->cellArr[_yIndex][_x] = _cell;
}

void Board_WriteLine(Board* _board, int _y, Cell* _line)
{
	const int _yIndex = (_board->height - 1) - _y;
	const int _byteSize = (int)_board->width * sizeof(wchar_t);

	memcpy_s(_board->cellArr[_yIndex], _byteSize, _line, _byteSize);
}

void Board_ClearCell(Board* _board, int _x, int _y)
{
	const int _yIndex = (_board->height - 1) - _y;
	const int _byteSize = sizeof(Cell);

	memset(&_board->cellArr[_yIndex][_x], 0, _byteSize);
}

void Board_ClearLine(Board* _board, int _y)
{
	const int _yIndex = (_board->height - 1) - _y;
	const int _byteSize = _board->width * sizeof(Cell);

	memset(_board->cellArr[_yIndex], 0, _byteSize);
}
