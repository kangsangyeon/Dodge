#pragma once
#include "WorldObject.h"
#include "Screen.h"

typedef struct TDirectionalBullet
{
	WorldObject* worldObject;
	float moveSpeed;
	Vector2D direction;
}DirectionalBullet;

DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, float _moveSpeed, Vector2D _direction);

void DirectionalBullet_Release(DirectionalBullet* _directionalBullet);

void DirectionalBullet_Move(DirectionalBullet* _directionalBullet, Vector2D _vector, float _deltatime);

void DirectionalBullet_RandomPositionCreate(int _width, int _heigth);
