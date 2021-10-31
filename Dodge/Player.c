#include "Player.h"

#include "Helper.h"
#include <stdlib.h>
#include <Windows.h>

#include "DodgeGameInstance.h"
#include "GameInstance.h"

Player* Player_Create(GameInstance* _gameInstance, wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position,
                      float _moveSpeed, double _dashCoolTime, float _dashSpeed, double _dashDuration, double _invincibleDuration)
{
	Player* _player = (Player*)malloc(sizeof(Player));
	_player->worldObject = WorldObject_CreateWithSpriteMask(NULL, NULL, NULL, _pivot, _position);
	_player->moveSpeed = _moveSpeed;
	_player->dashSpeed = _dashSpeed;
	_player->dashDuration = _dashDuration;
	_player->dashCoolTime = _dashCoolTime;
	_player->invincibleDuration = _invincibleDuration;

	// health
	_player->health = PLAYER_MAX_HEALTH;
	_player->healthSprite[0] = Sprite_LoadFromImageAndMaskFiles(L"Sprites/player_heart_half.txt", L"Sprites/player_heart_half.txt");
	_player->healthSprite[1] = Sprite_LoadFromImageAndMaskFiles(L"Sprites/player_heart.txt", L"Sprites/player_heart.txt");
	_player->healthCollider[0] = Collider_LoadFromTextFile(L"Sprites/player_heart_half.txt");
	_player->healthCollider[1] = Collider_LoadFromTextFile(L"Sprites/player_heart.txt");
	_player->flickerAnim = SpriteFlickerAnimation_Create(.075);

	_player->isInvincible = false;
	_player->lastDamagedTime = 0;

	_Player_UpdateSpriteAndCollider(_player);

	_player->playerBeHitClip = AudioClip_LoadFromFile(_gameInstance->audio, L"sounds/sfx/player_behit.wav", false);
	_player->playerDashClip = AudioClip_LoadFromFile(_gameInstance->audio, L"sounds/sfx/player_dash.wav", false);

	return _player;
}

void Player_Release(Player* _player)
{
	if (_player == NULL)
		return;

	if (_player->worldObject != NULL)
		WorldObject_Release(_player->worldObject);

	if (_player->healthSprite[0] != NULL)
		Sprite_Release(_player->healthSprite[0]);

	if (_player->healthSprite[1] != NULL)
		Sprite_Release(_player->healthSprite[1]);

	if (_player->healthCollider[0] != NULL)
		Collider_Release(_player->healthCollider[0]);

	if (_player->healthCollider[1] != NULL)
		Collider_Release(_player->healthCollider[1]);

	if (_player->flickerAnim != NULL)
		SpriteFlickerAnimation_Release(_player->flickerAnim);

	if (_player->playerBeHitClip != NULL)
		AudioClip_Release(_player->playerBeHitClip);

	if (_player->playerDashClip != NULL)
		AudioClip_Release(_player->playerDashClip);

	free(_player);
}

void Player_Initialize(Player* _player)
{
	if (_player == NULL)
		return;

	_player->health = 2;
	_Player_UpdateSpriteAndCollider(_player);
}

void _Player_MoveTick(GameInstance* _gameInstance, Player* _player, float _deltaTime)
{
	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	const int _screenWidth = _gameInstance->screen->width;
	const int _screenHeight = _gameInstance->screen->height;

	Vector2D _velocity = {0, 0};

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
		{
			Player_StartDash(_player, _velocity, _gameTime);
			Audio_Play(_gameInstance->audio, _player->playerDashClip, false);
		}
	}

	if (_player->isDash == true)
		Player_Move(_player, _player->dashDirection, _player->dashSpeed, _deltaTime, _screenWidth, _screenHeight);
	else
		Player_Move(_player, _velocity, _player->moveSpeed, _deltaTime, _screenWidth, _screenHeight);
}

void _Player_StateTick(GameInstance* _gameInstance, Player* _player)
{
	const double _gameTime = GameInstance_GetGameTime(_gameInstance);
	if (_player->isInvincible == true && _gameTime - _player->lastDamagedTime >= _player->invincibleDuration)
	{
		// 플레이어가 피격된 뒤 일정 시간동안 무적 상태가 되는데,
		// 무적시간이 지나면 무적과 깜빡임 효과를 종료합니다.
		_player->isInvincible = false;
		_player->lastDamagedTime = 0;
		SpriteFlickerAnimation_SetEnable(_player->flickerAnim, false, _gameTime);
	}
}

void Player_DrawTick(GameInstance* _gameInstance, Player* _player)
{
	if (_gameInstance == NULL || _player == NULL)
		return;

	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	if (_player->flickerAnim->enable)
	{
		SpriteFlickerAnimation_Tick(_player->flickerAnim, _gameTime);

		if (_player->flickerAnim->visible)
			Screen_PrintWorldObject(_gameInstance->screen, _player->worldObject);
	}
	else
	{
		Screen_PrintWorldObject(_gameInstance->screen, _player->worldObject);
	}
}

void Player_Tick(GameInstance* _gameInstance, Player* _player, float _deltaTime)
{
	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	_Player_MoveTick(_gameInstance, _player, _deltaTime);

	_Player_StateTick(_gameInstance, _player);
}

void Player_Move(Player* _player, Vector2D _vector, float _moveSpeed, float _deltaTime, int _width, int _height)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return;

	Vector2D _velocity = Vector2D_Multiply(_vector, _moveSpeed * _deltaTime);
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

void Player_Damaged(DodgeGameInstance* _dodgeGame, Player* _player, int _damage, double _gameTime)
{
	if (_player == NULL)
		return;

	// 플레이어가 무적 상태라면 데미지 처리를 무시합니다.
	if (_player->isInvincible == true)
		return;

	// 플레이어가 이미 죽었다면 데미지 처리를 무시합니다.
	if (_player->health <= 0)
		return;

	_player->health -= _damage;
	Audio_Play(_dodgeGame->gameInstance->audio, _player->playerBeHitClip, false);


	if (_player->health <= 0)
	{
		// 플레이어가 사망했을 때 게임오버 씬으로 전환합니다.
		_player->health = 0;

		DodgeGameInstance_ChangeScene(_dodgeGame, EST_GAMEOVER);
	}
	else
	{
		// 플레이어가 데미지를 입었지만 죽지 않았다면
		// 일정 시간동안 무적 상태가 되며 깜빡입니다.
		// 그것의 구현을 위해, 데미지를 입은 시간을 현재 시간으로 갱신합니다.
		_player->isInvincible = true;
		_player->lastDamagedTime = _gameTime;

		SpriteFlickerAnimation_SetEnable(_player->flickerAnim, true, _gameTime);

		_Player_UpdateSpriteAndCollider(_player);
	}
}

void _Player_UpdateSpriteAndCollider(Player* _player)
{
	// 체력에 맞는 sprite와 collider로 변경합니다.
	_player->worldObject->sprite = _player->healthSprite[_player->health - 1];
	_player->worldObject->collider = _player->healthCollider[_player->health - 1];
}
