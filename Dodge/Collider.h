﻿#pragma once
#include <stdbool.h>
#include <wchar.h>

#include "CollisionInfo.h"
#include "Vector2D.h"

typedef struct TCollider
{
	int width;
	int height;
	wchar_t** dataArr;
} Collider;

Collider* Collider_LoadFromTextFile(wchar_t* _filePath);

void Collider_Release(Collider* _collider);

bool Collider_CheckCollision(Collider* _source, Vector2D _sourcePosition, Vector2D _sourcePivot,
                             Collider* _other, Vector2D _otherPosition, Vector2D _otherPivot);

bool Collider_CheckCollisionDebug(Collider* _source, Vector2D _sourcePosition, Vector2D _sourcePivot,
                                  Collider* _other, Vector2D _otherPosition, Vector2D _otherPivot,
                                  CollisionInfo** _outCollisionInfo);
