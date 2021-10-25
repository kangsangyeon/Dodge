#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "PointerStack.h"
#include "Helper.h"
#include "IntQueue.h"

Sprite* Sprite_LoadFromImageFile(wchar_t* _imageFilePath)
{
	wchar_t** _imageArr = NULL;
	int _imageWidth = 0;
	int _imageHeight = 0;

	bool _success = LoadTextFileAsMatrix2D(_imageFilePath, &_imageArr, &_imageWidth, &_imageHeight);
	if (_success == false)
		return NULL;

	Sprite* _outSprite = (Sprite*)calloc(1, sizeof(Sprite));

	_outSprite->imageArr = _imageArr;
	_outSprite->imageWidth = _imageWidth;
	_outSprite->imageHeight = _imageHeight;

	return _outSprite;
}

Sprite* Sprite_LoadFromImageAndMaskFiles(wchar_t* _imageFilePath, wchar_t* _maskFilePath)
{
	wchar_t** _imageArr = NULL;
	int _imageWidth = 0;
	int _imageHeight = 0;

	bool _success = LoadTextFileAsMatrix2D(_imageFilePath, &_imageArr, &_imageWidth, &_imageHeight);
	if (_success == false)
		return NULL;

	bool** _maskBoolArr = NULL;
	int _maskWidth = 0;
	int _maskHeight = 0;

	_success = LoadTextFileAsBoolMatrix2D(_maskFilePath, &_maskBoolArr, &_maskWidth, &_maskHeight);
	if (_success == false)
		return NULL;

	// maskBoolArr을 매 줄마다 순회하여
	// 공백과 마스킹 문자가 연속되어 있는 개수를 순차적으로 구하는데,
	// 이 구조를 반복적으로 저장하는 배열을 만듭니다.
	int** _maskArr = (int**)malloc(_maskHeight * sizeof(int*));
	for (int _y = 0; _y < _maskHeight; ++_y)
	{
		IntQueue* _queue = IntQueue_Create(10);

		int _x = 0;
		while (_x < _maskWidth)
		{
			const int _spaceStartX = _x;
			int _spaceX = _spaceStartX + 1;
			for (; _spaceX < _maskWidth; ++_spaceX)
			{
				if (_maskBoolArr[_y][_spaceX] != false)
					break;
			}

			const int _continuousSpaceCount = _spaceX - _spaceStartX;
			IntQueue_Enqueue(_queue, _continuousSpaceCount);

			_x = _spaceX;

			if (_x >= _maskWidth)
			{
				IntQueue_Enqueue(_queue, 0);
				break;
			}

			const int _maskStartX = _spaceX;
			int _maskX = _maskStartX + 1;
			for (; _maskX < _maskWidth; ++_maskX)
			{
				if (_maskBoolArr[_y][_maskX] != true)
					break;
			}

			const int _continuousMaskCount = _maskX - _maskStartX;
			IntQueue_Enqueue(_queue, _continuousMaskCount);

			_x = _maskX;
		}

		int* _maskLine = (int*)malloc(IntQueue_GetSize(_queue) * sizeof(int));

		int i = 0;
		while (IntQueue_IsEmpty(_queue) == false)
		{
			int _popped = IntQueue_Dequeue(_queue);
			_maskLine[i] = _popped;
			i++;
		}

		IntQueue_Release(_queue);

		_maskArr[_y] = _maskLine;
	}

	Sprite* _outSprite = (Sprite*)calloc(1, sizeof(Sprite));

	_outSprite->imageArr = _imageArr;
	_outSprite->imageWidth = _imageWidth;
	_outSprite->imageHeight = _imageHeight;

	_outSprite->maskArr = _maskArr;
	_outSprite->maskHeight = _maskHeight;

	return _outSprite;
}

void Sprite_Release(Sprite* _sprite)
{
	if (_sprite == NULL)
		return;

	if (_sprite->imageArr != NULL)
	{
		for (int i = 0; i < _sprite->imageHeight; ++i)
		{
			if (_sprite->imageArr[i] != NULL)
				free(_sprite->imageArr[i]);
		}

		free(_sprite->imageArr);
	}

	if (_sprite->maskArr != NULL)
	{
		for (int i = 0; i < _sprite->maskHeight; ++i)
		{
			if (_sprite->maskArr[i] != NULL)
				free(_sprite->maskArr[i]);
		}

		free(_sprite->maskArr);
	}

	free(_sprite);
}
