#pragma once
#include "WorldObject.h"
#include "AudioClip.h"

typedef struct TDodgeGameInstance DodgeGameInstance;

typedef struct TScene_GameOver
{
	WorldObject* gameOverObject;
	AudioClip* gameOverBgmClip;
} Scene_GameOver;

Scene_GameOver* Scene_GameOver_Create(DodgeGameInstance* _dodgeGame);

void Scene_GameOver_Release(Scene_GameOver* _scene);

void Scene_GameOver_Tick(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);

void Scene_GameOver_OnEnter(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame);

void Scene_GameOver_OnExit(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame);
