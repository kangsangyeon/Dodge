#pragma once
#include <wchar.h>

typedef struct TCollider
{
	int width;
	int height;
	wchar_t** dataArr;
} Collider;

Collider* Collider_LoadFromTextFile(wchar_t* _filePath);

void Collider_Release(Collider* _collider);
