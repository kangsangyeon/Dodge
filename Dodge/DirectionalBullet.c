#include "DirectionalBullet.h"

#include <stdlib.h>

#include "DodgeGameInstance.h"
#include "Player.h"

DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot,
                                            Vector2D _position, Vector2D _direction, float _moveSpeed)
{
	DirectionalBullet* _directionalBullet = (DirectionalBullet*)malloc(sizeof(DirectionalBullet));
	_directionalBullet->worldObject = WorldObject_CreateWithSpriteMask(_spriteImageFilePath, _spriteMaskFilePath, _spriteMaskFilePath, _pivot, _position);
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

void DirectionalBullet_CollisionTick(DodgeGameInstance* _dodgeGame, DirectionalBullet* _bullet, Player* _player)
{
	if (_bullet == NULL || _player == NULL)
		return;

	const Collider* _bulletCollider = _bullet->worldObject->collider;
	const Vector2D _bulletPosition = _bullet->worldObject->position;
	const Vector2D _bulletPivot = _bullet->worldObject->pivot;

	const Collider* _playerCollider = _player->worldObject->collider;
	const Vector2D _playerPosition = _player->worldObject->position;
	const Vector2D _playerPivot = _player->worldObject->pivot;

	const bool _collisionResult = Collider_CheckCollision(_bulletCollider, _bulletPosition, _bulletPivot,
	                                                      _playerCollider, _playerPosition, _playerPivot);

	if (_collisionResult && _player->isInvincible == false)
	{
		const double _gameTime = GameInstance_GetGameTime(_dodgeGame->gameInstance);

		Player_Damaged(_dodgeGame, _player, 1, _gameTime);
	}
}

void DirectionalBullet_Move(DirectionalBullet* _directionalBullet, float _deltatime)
{
	if (_directionalBullet == NULL)
		return;

	if (Vector2D_IsEquals(_directionalBullet->direction, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_directionalBullet->direction, _directionalBullet->moveSpeed * _deltatime);
	_velocity.x = _velocity.x * 1.67;

	_directionalBullet->worldObject->position = Vector2D_Add(_directionalBullet->worldObject->position, _velocity);
}

Vector2D DirectionalBullet_CreateRandomPosition(int _width, int _height, Vector2D* _outDirection)
{
	const int createDirectionalBulletRange = 21;
	const int createDirectionalBulletCorrection = 10;
	const int randomWidthLine = rand() % (_width + createDirectionalBulletRange) - createDirectionalBulletCorrection;
	const int randomHeightLine = rand() % (_height + createDirectionalBulletRange) - createDirectionalBulletCorrection;

	Vector2D _randomExternalUpLine = {randomWidthLine, _height + createDirectionalBulletCorrection};
	Vector2D _randomExternalDownLine = {randomWidthLine, -createDirectionalBulletCorrection};
	Vector2D _randomExternalRightLine = {_width + createDirectionalBulletCorrection, randomHeightLine};
	Vector2D _randomExternalLeftLine = {-createDirectionalBulletCorrection, randomHeightLine};


	const int _upNdownLineDirectional = rand() % _width;
	const int _rightNLeftLineDirectional = rand() % _height;

	Vector2D _randomMiddleHorizontalLine = {_upNdownLineDirectional, (_height / 2)};
	Vector2D _randomMiddleVerticalLine = {(_width / 2), _rightNLeftLineDirectional};


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
		*_outDirection = Vector2D_Sub(_randomMiddleVerticalLine, _randomExternalRightLine);
		return _randomExternalRightLine;
	case 3:
		*_outDirection = Vector2D_Sub(_randomMiddleVerticalLine, _randomExternalLeftLine);
		return _randomExternalLeftLine;
	}
	return Vector2D_Zero;
}

bool DirectionalBullet_Destroy(DirectionalBullet* _directionalBullet, int _width, int _height)
{
	if (_directionalBullet == NULL)
		return false;

	const int _destroyRange = 15;
	Vector2D _checkToDestroyDirectionalBullet = _directionalBullet->worldObject->position;
	Vector2D _destoryScreenMinRange = {-_destroyRange, -_destroyRange};
	Vector2D _destoryScreenMaxRange = {_width + _destroyRange, _height + _destroyRange};

	if (_checkToDestroyDirectionalBullet.x < _destoryScreenMinRange.x ||
		_checkToDestroyDirectionalBullet.x > _destoryScreenMaxRange.x ||
		_checkToDestroyDirectionalBullet.y < _destoryScreenMinRange.y ||
		_checkToDestroyDirectionalBullet.y > _destoryScreenMaxRange.y)
	{
		DirectionalBullet_Release(_directionalBullet);
		return true;
	}
	return false;
}
