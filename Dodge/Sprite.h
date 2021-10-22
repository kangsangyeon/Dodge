#pragma once
#include <wchar.h>

typedef struct TSprite
{
	int width;
	int height;
	wchar_t** dataArr;
} Sprite;

Sprite* Sprite_LoadFromTextFile(wchar_t* _filePath);

void Sprite_Release(Sprite* _sprite);
