#pragma once
#include "DodgeGameInstance.h"
#include "WorldObject.h"
#include "AudioClip.h"

typedef struct TScene_GameClear
{
	WorldObject* gameClearObject;
	AudioClip* gameClearBgmClip;
} Scene_GameClear;

Scene_GameClear* Scene_GameClear_Create(DodgeGameInstance* _dodgeGame);

void Scene_GameClear_Release(Scene_GameClear* _scene);

void Scene_GameClear_Tick(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);

void Scene_GameClear_OnEnter(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame);

void Scene_GameClear_OnExit(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame);
