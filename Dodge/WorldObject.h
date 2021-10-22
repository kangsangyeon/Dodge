#pragma once
#include "Sprite.h"
#include "Vector2D.h"

typedef struct TWorldObject
{
	Sprite* sprite;
	Vector2D pivot;
	Vector2D position;
} WorldObject;

WorldObject* WorldObject_Create(Sprite* _sprite, Vector2D _pivot, Vector2D _position);

WorldObject* WorldObject_CreateWithTextFile(wchar_t* _spriteTextFilePath, Vector2D _pivot, Vector2D _position);

void WorldObject_Release(WorldObject* _worldObject);
