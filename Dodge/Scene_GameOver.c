#include "Scene_GameOver.h"

#include <stdlib.h>

#include "DodgeGameInstance.h"

Scene_GameOver* Scene_GameOver_Create(DodgeGameInstance* _dodgeGame)
{
	Scene_GameOver* _outScene = (Scene_GameOver*)malloc(sizeof(Scene_GameOver));

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

	free(_scene->gameOverObject);
}

void Scene_GameOver_Tick(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameOverObject != NULL)
		Screen_PrintWorldObject(_dodgeGame->gameInstance->screen, _scene->gameOverObject);

	if (GetAsyncKeyState(VK_SPACE))
		DodgeGameInstance_ChangeScene(_dodgeGame, EST_TITLE);
}

void Scene_GameOver_OnEnter(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void Scene_GameOver_OnExit(Scene_GameOver* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}
