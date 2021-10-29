#include "Player.h"

#include <stdlib.h>
#include <Windows.h>

Player* Player_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed, float _dashSpeed, double _dashDuration)
{
	Player* _player = (Player*)malloc(sizeof(Player));
	_player->worldObject = WorldObject_CreateWithSpriteMask(_spriteImageFilePath, _spriteMaskFilePath, _pivot, _position);
	_player->moveSpeed = _moveSpeed;
	_player->dashSpeed = _dashSpeed;
	_player->dashDuration = _dashDuration;

	return _player;
}

void Player_Release(Player* _player)
{
	if (_player == NULL)
		return;

	if (_player->worldObject != NULL)
		WorldObject_Release(_player->worldObject);

	free(_player);
}

void Player_Tick(Player* _player, float _deltaTime, float _gameTime)
{
	Vector2D _velocity = { 0, 0 };

	if (_player->isDash) {

		// 대시 지속시간이 끝났는지 확인하고,
		// 끝났다면 대시 관련 변수들을 초기화해준다. ex) isDash = false;로 바꾼다던지..
		const float _elapsedDashTime = _gameTime - _player->dashStartTime;
		const bool _isDashFinished = _elapsedDashTime >= _player->dashDuration;

		if (_isDashFinished == true) {
			// 대시를 끝낸다.
			_player->isDash = false;
			_player->dashStartTime = 0;
			_player->dashDirection = Vector2D_Zero;
			//
		}
	}

	if (_player->isDash == false) {

		if (GetAsyncKeyState(VK_RIGHT))
			_velocity.x += 1;

		if (GetAsyncKeyState(VK_LEFT))
			_velocity.x -= 1;

		if (GetAsyncKeyState(VK_UP))
			_velocity.y += 1;

		if (GetAsyncKeyState(VK_DOWN))
			_velocity.y -= 1;

		_velocity = Vector2D_Normalized(_velocity);

		if (GetAsyncKeyState(VK_SPACE))
			Player_StartDash(_player, _velocity, _gameTime);
	}

	if (_player->isDash == true)
		Player_Move(_player, _player->dashDirection, _player->dashSpeed, _deltaTime);
	else
		Player_Move(_player, _velocity, _player->moveSpeed, _deltaTime);
}

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return;

	const Vector2D _velocity = Vector2D_Multiply(_vector, _moveSpeed * _deltaTime);

	_player->worldObject->position = Vector2D_Add(_player->worldObject->position, _velocity);
}

void Player_StartDash(Player* _player, Vector2D _vector, float _gameTime)
{
	_player->isDash = true;
	_player->dashStartTime = _gameTime;
	_player->dashDirection = _vector;
}
