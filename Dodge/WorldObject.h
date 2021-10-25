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

WorldObject* WorldObject_CreateWithSprite(wchar_t* _spriteImageFilePath, Vector2D _pivot, Vector2D _position);

WorldObject* WorldObject_CreateWithSpriteMask(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position);

void WorldObject_Release(WorldObject* _worldObject);
