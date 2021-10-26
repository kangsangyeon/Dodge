#pragma once
#include <stdbool.h>

#include "Vector2D.h"

typedef struct TCollisionInfo
{
	Vector2D startPosition;
	int width;
	int height;
	bool** arr;
} CollisionInfo;

CollisionInfo* CollisionInfo_Create(Vector2D _startPosition, bool** _arr, int _width, int _height);

void CollisionInfo_Release(CollisionInfo* _collisionInfo);
