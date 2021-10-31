#pragma once
#include <stdbool.h>

#include "WorldObject.h"

typedef struct TDodgeGameInstance DodgeGameInstance;
typedef struct TPlayer Player;

typedef struct TDirectionalBullet
{
	WorldObject* worldObject;
	float moveSpeed;
	Vector2D direction;
} DirectionalBullet;

DirectionalBullet* DirectionalBullet_Create(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot,
                                            Vector2D _position, Vector2D _direction, float _moveSpeed);

void DirectionalBullet_Release(DirectionalBullet* _directionalBullet);

void DirectionalBullet_CollisionTick(DodgeGameInstance* _dodgeGame, DirectionalBullet* _bullet, Player* _player);

void DirectionalBullet_Move(DirectionalBullet* _directionalBullet, float _deltatime);

Vector2D DirectionalBullet_CreateRandomPosition(int _width, int _height, Vector2D* _outDirection);

bool DirectionalBullet_Destroy(DirectionalBullet* _directionalBullet, int _width, int _height);
