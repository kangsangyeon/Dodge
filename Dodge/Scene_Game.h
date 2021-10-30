#pragma once

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
	EGS_BOSS_PLAYING = 2,
	EGS_BOSS_WAITING = 3
} EGameState;

typedef enum TEBossType
{
	EBT_NONE = 0,
	EBT_DOGE_MUSK = 1,
	EBT_MAX = 2
} EBossType;

typedef struct TScene_Game
{
	Player* player;
	EBossType bossType;
	Boss_DogeMusk* dogeMusk;
	DirectionalBullet* directionalBullet;
	LinearBullet* linearBullet;
} Scene_Game;

Scene_Game* Scene_Game_Create(DodgeGameInstance* _dodgeGame);

void Scene_Game_Release(Scene_Game* _scene);

void Scene_Game_OnEnter(Scene_Game* _scene, DodgeGameInstance* _dodgeGame);

void Scene_Game_OnExit(Scene_Game* _scene, DodgeGameInstance* _dodgeGame);

void Scene_Game_Tick(Scene_Game* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);
