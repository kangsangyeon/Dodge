#pragma once
#include <stdbool.h>
#include <wchar.h>

typedef struct TSprite
{
	wchar_t** imageArr;
	int imageWidth;
	int imageHeight;

	int** maskArr;
	int maskHeight;
} Sprite;

Sprite* Sprite_LoadFromImageFile(wchar_t* _imageFilePath);

Sprite* Sprite_LoadFromImageAndMaskFiles(wchar_t* _imageFilePath, wchar_t* _maskFilePath);

void Sprite_Release(Sprite* _sprite);
