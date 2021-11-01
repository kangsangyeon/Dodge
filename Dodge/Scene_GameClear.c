#include "Scene_GameClear.h"

#include "Const.h"

Scene_GameClear* Scene_GameClear_Create(DodgeGameInstance* _dodgeGame)
{
	Scene_GameClear* _outScene = (Scene_GameClear*)malloc(sizeof(Scene_GameClear));

	_outScene->gameClearBgmClip = AudioClip_LoadFromFile(_dodgeGame->gameInstance->audio, L"Sounds/Bgm/gameclear_bgm.wav", false);

	const Vector2D _screenCenter = {_dodgeGame->gameInstance->screen->width / 2, _dodgeGame->gameInstance->screen->height / 2};
	_outScene->gameClearObject = WorldObject_CreateWithSprite(L"Sprites/Scene/Scene_gameclear.txt", NULL, Vector2D_Center, _screenCenter);

	return _outScene;
}

void Scene_GameClear_Release(Scene_GameClear* _scene)
{
	if (_scene == NULL)
		return;

	if (_scene->gameClearObject != NULL)
		WorldObject_Release(_scene->gameClearObject);

	if (_scene->gameClearBgmClip != NULL)
		AudioClip_Release(_scene->gameClearBgmClip);

	free(_scene);
}

void Scene_GameClear_Tick(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameClearObject != NULL)
		Screen_PrintWorldObject(_dodgeGame->gameInstance->screen, _scene->gameClearObject);

	if (GetAsyncKeyState(VK_SPACE) == KEY_STATE_DOWN)
		DodgeGameInstance_ChangeScene(_dodgeGame, EST_TITLE);
}

void Scene_GameClear_OnEnter(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameClearBgmClip != NULL)
		Audio_Play(_dodgeGame->gameInstance->audio, _scene->gameClearBgmClip, true);
}

void Scene_GameClear_OnExit(Scene_GameClear* _scene, DodgeGameInstance* _dodgeGame)
{
	if (_scene == NULL || _dodgeGame == NULL)
		return;

	if (_scene->gameClearBgmClip != NULL)
		Audio_Stop(_scene->gameClearBgmClip);
}
