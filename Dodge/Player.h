﻿#pragma once
#include "Collider.h"
#include "SpriteFlickerAnimation.h"
#include "WorldObject.h"
#include "AudioClip.h"

#define PLAYER_MAX_HEALTH 2

typedef struct TGameInstance GameInstance;
typedef struct TDodgeGameInstance DodgeGameInstance;

typedef struct TPlayer
{
	WorldObject* worldObject;
	float moveSpeed;
	float dashSpeed;
	double dashCoolTime;
	double dashDuration;
	double invincibleDuration;

	// for dash implement
	bool isDash;
	double dashStartTime;
	double dashEndTime;
	Vector2D dashDirection;

	// health
	int health;
	Sprite* healthSprite[PLAYER_MAX_HEALTH];
	Collider* healthCollider[PLAYER_MAX_HEALTH];
	SpriteFlickerAnimation* flickerAnim;

	// for health implement
	bool isInvincible;
	double lastDamagedTime;

	// Audio
	AudioClip* playerBeHitClip;
	AudioClip* playerDashClip;

} Player;

Player* Player_Create(GameInstance* _gameInstance, wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position,
	float _moveSpeed, double _dashCoolTime, float _dashSpeed, double _dashDuration, double _invincibleDuration);

void Player_Release(Player* _player);

void Player_Initialize(Player* _player);

void _Player_MoveTick(GameInstance* _gameInstance, Player* _player, float _deltaTime);

void _Player_StateTick(GameInstance* _gameInstance, Player* _player);

void Player_DrawTick(GameInstance* _gameInstance, Player* _player);

void Player_Tick(GameInstance* _gameInstance, Player* _player, float _deltaTime);

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime, int _width, int _height);

void Player_StartDash(Player* _player, Vector2D _vector, float _gameTime);

void Player_Damaged(DodgeGameInstance* _dodgeGame, Player* _player, int _damage, double _gameTime);

void _Player_UpdateSpriteAndCollider(Player* _player);
