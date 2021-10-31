#include "LinearBullet.h"

#include <stdlib.h>


LinearBullet* LinearBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, Vector2D _direction, float _moveSpeed)
{
	LinearBullet* _linearBullet = (LinearBullet*)malloc(sizeof(LinearBullet));
	_linearBullet->worldObject = WorldObject_CreateWithSprite(_spriteFilePath, _spriteFilePath, _pivot, _position);
	_linearBullet->moveSpeed = _moveSpeed;
	_linearBullet->direction = Vector2D_Normalized(_direction);
	return _linearBullet;
}

void LinearBullet_Release(LinearBullet* _linearBullet)
{
	if (_linearBullet == NULL)
		return;

	if (_linearBullet->worldObject != NULL)
		WorldObject_Release(_linearBullet->worldObject);

	free(_linearBullet);
}

void LinearBullet_Move(LinearBullet* _linearBullet, float _deltatime)
{
	if (_linearBullet == NULL)
		return;

	if (Vector2D_IsEquals(_linearBullet->direction, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_linearBullet->direction, _linearBullet->moveSpeed * _deltatime);
	_velocity.x = _velocity.x * 1.67;

	_linearBullet->worldObject->position = Vector2D_Add(_linearBullet->worldObject->position, _velocity);
}

Vector2D LinearBullet_CreatRandomPosition(int _width, int _height, Vector2D* _outDirection)
{
	const int randomWidthLinde = rand() % (_width - 100);
	const int randomHightLinde = rand() % (_height - 50);

	Vector2D _randomExternalUpLine = { randomWidthLinde, _height + 10 };
	Vector2D _randomExternalDownLine = { randomWidthLinde, -10 };
	Vector2D _randomExternalRightLine = { _width + 10, randomHightLinde };
	Vector2D _randomExternalLeftLine = { -10, randomHightLinde };

	const int randomExternalLineSelect = rand() % 4;
	switch (randomExternalLineSelect)
	{
	case 0:
		*_outDirection = Vector2D_Down;
		return _randomExternalUpLine;
	case 1:
		*_outDirection = Vector2D_Up;
		return _randomExternalDownLine;
	case 2:
		*_outDirection = Vector2D_Left;
		return _randomExternalRightLine;
	case 3:
		*_outDirection = Vector2D_Right;
		return _randomExternalLeftLine;
	}
	return Vector2D_Zero;
}