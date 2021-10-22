#pragma once
#include <wchar.h>

typedef struct TSprite
{
	int width;
	int height;
	wchar_t** dataArr;
} Sprite;

Sprite* loadSpriteFromText(wchar_t* _filePath);

void releaseSprite(Sprite* _sprite);
