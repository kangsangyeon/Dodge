#pragma once

typedef struct TDodgeGameInstance DodgeGameInstance;

typedef struct TScene_Title
{
	char temp;
} Scene_Title;

Scene_Title* Scene_Title_Create(DodgeGameInstance* _dodgeGame);

void Scene_Title_Release(Scene_Title* _scene);

void Scene_Title_Tick(Scene_Title* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime);

void Scene_Title_OnEnter(Scene_Title* _scene, DodgeGameInstance* _dodgeGame);

void Scene_Title_OnExit(Scene_Title* _scene, DodgeGameInstance* _dodgeGame);
