#include "Scene_Title.h"

#include <stdlib.h>


Scene_Title* Scene_Title_Create(DodgeGameInstance* _dodgeGame)
{
	Scene_Title* _outScene = (Scene_Title*)malloc(sizeof(Scene_Title));

	return _outScene;
}

void Scene_Title_Release(Scene_Title* _scene)
{
	if (_scene == NULL)
		return;

	free(_scene);
}

void Scene_Title_Tick(Scene_Title* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void Scene_Title_OnEnter(Scene_Title* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}

void Scene_Title_OnExit(Scene_Title* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;
}
