#pragma once
#include <wchar.h>

#include "Board.h"
#include "Boss_DogeMusk.h"
#include "Collider.h"
#include "GameInstance.h"
#include "Player.h"
#include "DirectionalBullet.h"
#include "LinearBullet.h"
#include "WarningSign.h"

typedef enum TEGameState
{
	EGS_NONE = 0,
	EGS_START = 1,
	EGS_BOSS_PLAYING = 2,
	EGS_BOSS_WAITING = 3
} EGameState;

typedef enum TEBossType
{
	EBT_NONE = 0,
	EBT_DOGE_MUSK = 1,
	EBT_MAX = 2
} EBossType;

typedef struct TDodgeGameInstance
{
	GameInstance* gameInstance;

	int screenWidth;
	int screenHeight;

	Player* player;
	EBossType bossType;
	Boss_DogeMusk* dogeMusk;
	DirectionalBullet* directionalBullet;
	LinearBullet* linearBullet;
} DodgeGameInstance;

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps);

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame);

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame);

void _DodgeGameInstance_GameTick(DodgeGameInstance* _dodgeGame, float _deltaTime);
