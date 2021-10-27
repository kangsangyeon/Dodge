#include "DirectionalBullet.h"

#include <stdlib.h>

DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, Vector2D _direction, float _moveSpeed)
{
	DirectionalBullet* _directionalBullet = (DirectionalBullet*)malloc(sizeof(DirectionalBullet));
	_directionalBullet->worldObject = WorldObject_CreateWithSprite(_spriteFilePath, _pivot, _position);
	_directionalBullet->moveSpeed = _moveSpeed;
	_directionalBullet->direction = Vector2D_Normalized(_direction);

	return _directionalBullet;
}

void DirectionalBullet_Release(DirectionalBullet* _directionalBullet)
{
	if (_directionalBullet == NULL)
		return;

	if (_directionalBullet->worldObject != NULL)
		WorldObject_Release(_directionalBullet->worldObject);

	free(_directionalBullet);
}

void DirectionalBullet_Move(DirectionalBullet* _directionalBullet, float _deltatime)
{
	if (Vector2D_IsEquals(_directionalBullet->direction, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_directionalBullet->direction, _directionalBullet->moveSpeed * _deltatime);
	_directionalBullet->worldObject->position = Vector2D_Add(_directionalBullet->worldObject->position, _velocity);
}


Vector2D DirectionalBullet_CreateRandomPosition(int _width, int _height, Vector2D* _outDirection)
{
	// 폭과 높이의 값으로 외곽 라인값들을 뽑는다.
	int randomWidthLine = rand() % (_width + 21) - 10;
	int randomHeightLine = rand() % (_height + 21) - 10;

	Vector2D _randomExternalUpLine = { randomWidthLine, _height + 10 };
	Vector2D _randomExternalDownLine = { randomWidthLine, -10 };
	Vector2D _randomExternalRightLine = { _width + 10 , randomHeightLine };
	Vector2D _randomExternalLeftLine = { -10, randomHeightLine };


	int _upNdownLineDirectional = rand() % _width;
	int _rightNLeftLineDirectional = rand() % _height;

	Vector2D _randomMiddleHorizontalLine = { _upNdownLineDirectional, (_height / 2) };
	Vector2D _randomMiddleVerticalLine = { (_width / 2), _rightNLeftLineDirectional };

	// 뽑은 값으로 1, 2, 3, 4 만듬
	// 각 분기별로 설정된 라인의 변동값을 랜덤으로 받는다.
	int randomExternalLineSelect = rand() % 4;

	switch (randomExternalLineSelect)
	{
	case 0:
		*_outDirection = Vector2D_Sub(_randomMiddleHorizontalLine, _randomExternalUpLine);
		return _randomExternalUpLine;
	case 1:
		*_outDirection = Vector2D_Sub(_randomMiddleHorizontalLine, _randomExternalDownLine);
		return _randomExternalDownLine;
	case 2:
		*_outDirection = Vector2D_Sub(_randomMiddleVerticalLine, _randomExternalLeftLine);
		return _randomExternalRightLine;
	case 3:
		*_outDirection = Vector2D_Sub(_randomMiddleVerticalLine, _randomExternalRightLine);
		return _randomExternalLeftLine;
	}

	return Vector2D_Zero;
}
