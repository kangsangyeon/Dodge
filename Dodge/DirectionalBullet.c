#include "DirectionalBullet.h"
#include "Screen.h"

#include <stdlib.h>




DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteFilePath, Vector2D _pivot,
	Vector2D _position, float _moveSpeed, Vector2D _direction)
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


void DirectionalBullet_RandomPositionCreate(int _width, int _heigth)
{


	// 1. 폭과 높이의 값으로 외곽 라인값을 뽑는다.
	// 내가 랜덤값을 뽑아야 하는데 .. 아니근데 벡터에 넣어야한단 말이지 그 랜덤 값을.. 그걸 참.. 어ㅏ케 넣는지..흠

	int randomOne = rand() % (_width + 21) - 10;
	Vector2D _temp = { randomOne, _heigth + 10 };



	// 2. 뽑은 값으로 1, 2, 3, 4 만듬

	// 3. 4개의 선택지 중 하나로 분기를 타자!

	// 4. 각 분기별로 설정된 라인의 변동값을 랜덤으로 받는다.


}