#pragma once
#include <stdbool.h>
#include <wchar.h>
#include "AudioClip.h"

typedef struct TGameInstance GameInstance;
typedef struct TDodgeGameInstance DodgeGameInstance;
typedef struct TPlayer Player;
typedef struct TBoss_DogeMusk Boss_DogeMusk;
typedef struct TDirectionalBullet DirectionalBullet;
typedef struct TLinearBullet LinearBullet;

typedef enum TEGameState
{
	EGS_NONE = 0,
	EGS_START = 1,
	EGS_WAITING = 2,
	EGS_NORMAL_EASY = 3,
	EGS_CHARACTER_EASY = 4,
	EGS_NORMAL_NORMAL = 5,
	EGS_CHARACTER_NORMAL = 6,
	EGS_NORMAL_HARD = 7,
	EGS_CHARACTER_HARD = 8,
	EGS_BOSS = 9,
	EGS_CLEAR = 10
} EGameState;

typedef enum TEGameBossState
{
	EGBS_NONE = 0,
	EGBS_START = 1,
	EGBS_PLAYING = 2,
	EGBS_WAITING = 3
} EGameBossState;

typedef enum TEBossType
{
	EBT_NONE = 0,
	EBT_DOGE_MUSK = 1,
	EBT_MAX = 2
} EBossType;

typedef struct TScene_Game
{
	double enterSceneTime;
	EGameState gameState;
	double gameStateStartTime;

	double lastSpawnBulletTime;

	EGameBossState gameBossState;
	EBossType bossType;

	Player* player;
	Boss_DogeMusk* dogeMusk;

	DirectionalBullet* bulletArr[50];

	AudioClip* ingameBgmClip;
} Scene_Game;

extern const int g_GameStateDurations[10];

extern const int g_GameStateStartBulletCount[10];

extern const int g_GameStateMaxBulletCount[10];

extern const int g_GameStateSpawnBulletDelay[10];

extern const float g_GameStateBulletSpeed[10];

extern const wchar_t** g_GameStateBulletSpriteFile[10];


Scene_Game* Scene_Game_Create(DodgeGameInstance* _dodgeGame);

void Scene_Game_Release(Scene_Game* _scene);

void Scene_Game_OnEnter(Scene_Game* _scene, DodgeGameInstance* _dodgeGame);

void Scene_Game_OnExit(Scene_Game* _scene, DodgeGameInstance* _dodgeGame);

void Scene_Game_Tick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);

bool _Scene_Game_CheckStateTransitionTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);

void _Scene_Game_StartStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart);

void _Scene_Game_NormalStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart);

void _Scene_Game_BossStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart);

void _Scene_Game_ClearStateTick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime, bool _stateStart);

void ReleaseBullets(Scene_Game* _scene);

void SpawnBullets(Scene_Game* _scene, int _screenWidth, int _screenHeight);

DirectionalBullet* SpawnBullet(Scene_Game* _scene, int _screenWidth, int _screenHeight);
