#include "Scene_Title.h"

#include <stdlib.h>

#include "DodgeGameInstance.h"


Scene_Title* Scene_Title_Create(DodgeGameInstance* _dodgeGame)
{
	Scene_Title* _outScene = (Scene_Title*)malloc(sizeof(Scene_Title));

	const Vector2D _screenCenter = {_dodgeGame->gameInstance->screen->width / 2, _dodgeGame->gameInstance->screen->height / 2};
	_outScene->titleObject = WorldObject_CreateWithSprite(L"Sprites/Scene/Scene_title.txt", NULL, Vector2D_Center, _screenCenter);

	return _outScene;
}

void Scene_Title_Release(Scene_Title* _scene)
{
	if (_scene == NULL)
		return;

	if (_scene->titleObject != NULL)
		WorldObject_Release(_scene->titleObject);

	free(_scene);
}

void Scene_Title_Tick(Scene_Title* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->titleObject != NULL)
		Screen_PrintWorldObject(_dodgeGame->gameInstance->screen, _scene->titleObject);

	if (GetAsyncKeyState(VK_SPACE))
	{
		// 스페이스바를 눌러 Game Scene으로 전환합니다.
		DodgeGameInstance_ChangeScene(_dodgeGame, EST_GAME);
	}
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
