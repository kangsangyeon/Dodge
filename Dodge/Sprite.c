#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "PointerStack.h"
#include "Helper.h"

Sprite* Sprite_LoadFromTextFile(wchar_t* _filePath)
{
	wchar_t** _dataArr = NULL;
	int _spriteWidth = 0;
	int _spriteHeight = 0;

	bool _success = LoadTextFileAsMatrix2D(_filePath, &_dataArr, &_spriteWidth, &_spriteHeight);
	if (_success == false)
		return NULL;

	Sprite* _outSprite = (Sprite*)calloc(1, sizeof(Sprite));

	_outSprite->dataArr = _dataArr;
	_outSprite->width = _spriteWidth;
	_outSprite->height = _spriteHeight;

	return _outSprite;
}

void Sprite_Release(Sprite* _sprite)
{
	if (_sprite == NULL)
		return;

	if (_sprite->dataArr != NULL)
	{
		for (int i = 0; i < _sprite->height; ++i)
		{
			if (_sprite->dataArr[i] != NULL)
				free(_sprite->dataArr[i]);
		}

		free(_sprite->dataArr);
	}

	free(_sprite);
}
