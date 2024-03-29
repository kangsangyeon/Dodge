﻿#include "Scene_GameOver.h"

#include <stdlib.h>

#include "Const.h"
#include "DodgeGameInstance.h"

Scene_GameOver* Scene_GameOver_Create(DodgeGameInstance* _dodgeGame)
{
	Scene_GameOver* _outScene = (Scene_GameOver*)malloc(sizeof(Scene_GameOver));

	_outScene->gameOverBgmClip = AudioClip_LoadFromFile(_dodgeGame->gameInstance->audio, L"Sounds/Bgm/gameover_bgm.wav", false);

	const Vector2D _screenCenter = {_dodgeGame->gameInstance->screen->width / 2, _dodgeGame->gameInstance->screen->height / 2};
	_outScene->gameOverObject = WorldObject_CreateWithSprite(L"Sprites/Scene/Scene_gameover.txt", NULL, Vector2D_Center, _screenCenter);


	return _outScene;
}

void Scene_GameOver_Release(Scene_GameOver* _scene)
{
	if (_scene == NULL)
		return;

	if (_scene->gameOverObject != NULL)
		WorldObject_Release(_scene->gameOverObject);

	if (_scene->gameOverBgmClip != NULL)
		AudioClip_Release(_scene->gameOverBgmClip);

	free(_scene->gameOverObject);
}

void Scene_GameOver_Tick(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameOverObject != NULL)
		Screen_PrintWorldObject(_dodgeGame->gameInstance->screen, _scene->gameOverObject);

	bool _previousSpacePressed = _scene->spacePressed;
	_scene->spacePressed = GetAsyncKeyState(VK_SPACE);

	if (_scene->spacePressed == false && _previousSpacePressed == true)
		DodgeGameInstance_ChangeScene(_dodgeGame, EST_TITLE);
}

void Scene_GameOver_OnEnter(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameOverBgmClip != NULL && _dodgeGame->diedByCharacter == false)
		Audio_Play(_dodgeGame->gameInstance->audio, _scene->gameOverBgmClip, true);

	_scene->spacePressed = false;
}

void Scene_GameOver_OnExit(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameOverBgmClip != NULL && _dodgeGame->diedByCharacter == false)
		Audio_Stop(_scene->gameOverBgmClip);
}
