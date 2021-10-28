#pragma once

#include "WorldObject.h"

typedef struct TLinearBullet
{
	WorldObject* worldObject;
	float moveSpeed;
	Vector2D direction;
}LinearBullet;

LinearBullet* LinearBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, Vector2D _direction, float _moveSpeed);

void LinearBullet_Release(LinearBullet* _linearBullet);

void LinearBullet_Move(LinearBullet* _linearBullet, float _deltatime);

Vector2D LinearBullet_CreatRandomPosition(int _width, int _height, Vector2D* _outDirection);