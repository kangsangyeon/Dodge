#include "Player.h"
#include <Windows.h>

#include <stdlib.h>

Player* Player_Create(wchar_t* _spriteFilePath, Vector2D _pivot, Vector2D _position, float _moveSpeed)
{
	Player* _player = (Player*)malloc(sizeof(Player));
	_player->worldObject = WorldObject_CreateWithTextFile(_spriteFilePath, _pivot, _position);
	_player->moveSpeed = _moveSpeed;

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

void Player_Move(Player* _player, Vector2D _vector, float _deltaTime)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return;

	if (GetAsyncKeyState(VK_LEFT))
	{
		Vector2D _velocity = Vector2D_Multiply(_vector, _player->moveSpeed * _deltaTime);
		_player->worldObject->position = Vector2D_Sub(_player->worldObject->position, _velocity);
	}


	// 수정한 부분
	if (GetAsyncKeyState(VK_RIGHT))
	{
		Vector2D _velocity = Vector2D_Multiply(_vector, _player->moveSpeed * _deltaTime);
		_player->worldObject->position = Vector2D_Add(_player->worldObject->position, _velocity);
	}

}