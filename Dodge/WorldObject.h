#pragma once
#include "Collider.h"
#include "Sprite.h"
#include "Vector2D.h"

typedef struct TWorldObject
{
	Sprite* sprite;
	Collider* collider;
	Vector2D pivot;
	Vector2D position;
} WorldObject;

WorldObject* WorldObject_Create(Sprite* _sprite, Collider* _collider, Vector2D _pivot, Vector2D _position);

WorldObject* WorldObject_CreateWithSprite(wchar_t* _spriteImageFilePath, wchar_t* _colliderFilePath, Vector2D _pivot, Vector2D _position);

WorldObject* WorldObject_CreateWithSpriteMask(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, wchar_t* _colliderFilePath, Vector2D _pivot, Vector2D _position);

void WorldObject_Release(WorldObject* _worldObject);
