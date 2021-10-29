#pragma once
#include "WorldObject.h"

typedef struct TPlayer
{
	WorldObject* worldObject;
	float moveSpeed;
	float dashSpeed;
	double dashCoolTime;
	double dashDuration;

	bool isDash;
	double dashStartTime;
	double dashEndTime;
	Vector2D dashDirection;
} Player;

Player* Player_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed, double _dashCoolTime, float _dashSpeed, double _dashDuration);

void Player_Release(Player* _player);

void Player_Tick(Player* _player, float _deltaTime, float _gameTime, int _width, int _height);

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime, int _width, int _height);

void Player_StartDash(Player* _player, Vector2D _vector, float _gameTime);
