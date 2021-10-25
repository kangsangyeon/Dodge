#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "PointerStack.h"
#include "Helper.h"

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

	bool** _maskArr = NULL;
	int _maskWidth = 0;
	int _maskHeight = 0;

	_success = LoadTextFileAsBoolMatrix2D(_maskFilePath, &_maskArr, &_maskWidth, &_maskHeight);
	if (_success == false)
		return NULL;

	Sprite* _outSprite = (Sprite*)calloc(1, sizeof(Sprite));

	_outSprite->imageArr = _imageArr;
	_outSprite->imageWidth = _imageWidth;
	_outSprite->imageHeight = _imageHeight;

	_outSprite->maskArr = _maskArr;
	_outSprite->maskWidth = _maskWidth;
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
