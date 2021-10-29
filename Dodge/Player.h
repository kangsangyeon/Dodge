#pragma once
#include "WorldObject.h"

typedef struct TPlayer
{
	WorldObject* worldObject;
	float moveSpeed;

	bool isDash;
	float dashSpeed;
	double dashStartTime;
	double dashDuration;
	Vector2D dashDirection;
	float dashCoolTime;
} Player;

Player* Player_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed, float _dashSpeed, double _dashDuration);

void Player_Release(Player* _player);

void Player_Tick(Player* _player, float _deltaTime, float _gameTime);

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime);

void Player_StartDash(Player* _player, Vector2D _vector, float _gameTime);
