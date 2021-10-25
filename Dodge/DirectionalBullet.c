#include "DirectionalBullet.h"

#include <stdlib.h>

DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, float _moveSpeed)
{
	DirectionalBullet* _directionalBullet = (DirectionalBullet*)malloc(sizeof(DirectionalBullet));
	_directionalBullet->worldObject = WorldObject_CreateWithSprite(_spriteFilePath, _pivot, _position);
	_directionalBullet->moveSpeed = _moveSpeed;

	

	
	// 5. 총알을 뿌려준다!

	// 6. 해당 사이클은 게임이 종료될때 까지 진행된다. (총알을 꼐속 뿌려줄 예정)




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

void DirectionalBullet_Move(DirectionalBullet* _directionalBullet, Vector2D _vector, float _deltatime)
{

	// rand() % 51 +50 ;
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_vector, _directionalBullet->moveSpeed * _deltatime);
	_directionalBullet->worldObject->position = Vector2D_Add(_directionalBullet->worldObject->position, _velocity);
}


void DirectionalBullet_RandomPositionCreate(int _width, int _height, Vector2D _direction)
{
	// 폭과 높이의 값으로 외곽 라인값들을 뽑는다.
	int randomWidthLine = rand() % (_width + 21) - 10;
	int randomHeightLine = rand() % (_height + 21) - 10;

	Vector2D _randomExternalUpLine = { randomWidthLine, _height + 10 };
	Vector2D _randomExternalDownLine = { randomWidthLine, _height - 210 };
	Vector2D _randomExternalRightLine = { _width - 210, randomHeightLine };
	Vector2D _randomExternalLeftLine = { _width + 10, randomHeightLine };

	// 뽑은 값으로 1, 2, 3, 4 만듬
	int randomExternalLineSelect = rand() % 4;
	int randomExternalLineRetun = 0;
	if (randomExternalLineSelect == randomExternalLineSelect)
		return;

	// 각 분기별로 설정된 라인의 변동값을 랜덤으로 받는다.
	switch (randomExternalLineSelect)
	{
		case 0: _randomExternalUpLine; break;
		case 1: _randomExternalDownLine; break;
		case 2: _randomExternalRightLine; break;
		case 3: _randomExternalLeftLine; break;
	}
	// 이걸로는 안되지 않나..? 일단 보류..
}

// void DirectionalBullet_