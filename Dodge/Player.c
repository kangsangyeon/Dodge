#include "Player.h"

#include "Helper.h"
#include <stdlib.h>
#include <Windows.h>

Player* Player_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed, double _dashCoolTime, float _dashSpeed, double _dashDuration)
{
	Player* _player = (Player*)malloc(sizeof(Player));
	_player->worldObject = WorldObject_CreateWithSpriteMask(_spriteImageFilePath, _spriteMaskFilePath, _pivot, _position);
	_player->moveSpeed = _moveSpeed;
	_player->dashSpeed = _dashSpeed;
	_player->dashDuration = _dashDuration;
	_player->dashCoolTime = _dashCoolTime;

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

void Player_Tick(Player* _player, float _deltaTime, float _gameTime, int _width, int _height)
{
	Vector2D _velocity = { 0, 0 };

	if (_player->isDash)
	{
		// 대시 지속시간이 끝났는지 확인하고,
		// 끝났다면 대시 관련 변수들을 초기화해준다. ex) isDash = false;로 바꾼다던지..
		const float _elapsedDashTime = _gameTime - _player->dashStartTime;
		const bool _isDashFinished = _elapsedDashTime >= _player->dashDuration;

		if (_isDashFinished == true)
		{
			// 대시를 끝낸다.
			_player->isDash = false;
			_player->dashStartTime = 0;
			_player->dashDirection = Vector2D_Zero;
			_player->dashEndTime = _gameTime;
		}
	}

	if (_player->isDash == false)
	{
		if (GetAsyncKeyState(VK_RIGHT))
			_velocity.x += 1;

		if (GetAsyncKeyState(VK_LEFT))
			_velocity.x -= 1;

		if (GetAsyncKeyState(VK_UP))
			_velocity.y += 1;

		if (GetAsyncKeyState(VK_DOWN))
			_velocity.y -= 1;

		_velocity = Vector2D_Normalized(_velocity);

		double _elapsedTimeAfterDashEnd = _gameTime - _player->dashEndTime;
		bool _isDashReady = _elapsedTimeAfterDashEnd >= _player->dashCoolTime;

		if (GetAsyncKeyState(VK_SPACE) && _isDashReady == true)
			Player_StartDash(_player, _velocity, _gameTime);
	}

	if (_player->isDash == true)
		Player_Move(_player, _player->dashDirection, _player->dashSpeed, _deltaTime, _width, _height);
	else
		Player_Move(_player, _velocity, _player->moveSpeed, _deltaTime, _width, _height);
}

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime, int _width, int _height)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_vector, _moveSpeed *_deltaTime);
	_velocity.x = _velocity.x * 1.67;

	_player->worldObject->position = Vector2D_Add(_player->worldObject->position, _velocity);

	// 화면 밖으로 나가는 것을 보정
	int correctionWidth = _player->worldObject->sprite->imageWidth / 2;
	int correctionHeight = _player->worldObject->sprite->imageHeight / 2;
	Vector2D _getPlayerVector = _player->worldObject->position;
	_player->worldObject->position.x = FClamp(_getPlayerVector.x, correctionWidth, _width - correctionWidth);
	_player->worldObject->position.y = FClamp(_getPlayerVector.y, correctionHeight, _height - correctionHeight);
}

void Player_StartDash(Player* _player, Vector2D _vector, float _gameTime)
{
	_player->isDash = true;
	_player->dashStartTime = _gameTime;
	_player->dashDirection = _vector;
}
