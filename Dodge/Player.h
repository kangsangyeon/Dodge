#pragma once
#include "WorldObject.h"

typedef struct TPlayer
{
	WorldObject* worldObject;
	float moveSpeed;
} Player;

Player* Player_Create(wchar_t* _spriteFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed);

void Player_Release(Player* _player);

void Player_Move(Player* _player, Vector2D _vector, float _deltaTime);
