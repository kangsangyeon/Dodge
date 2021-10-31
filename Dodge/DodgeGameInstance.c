#include "DodgeGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>

#include "Scene_Game.h"
#include "Scene_GameClear.h"
#include "Scene_GameOver.h"
#include "Scene_Title.h"

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps)
{
	DodgeGameInstance* _instance = (DodgeGameInstance*)calloc(1, sizeof(DodgeGameInstance));
	_instance->gameInstance = GameInstance_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _foregroundColor, _backgroundColor, _useColor, _desiredFps);

	_instance->titleScene = Scene_Title_Create(_instance);
	_instance->gameScene = Scene_Game_Create(_instance);
	_instance->gameOverScene = Scene_GameOver_Create(_instance);
	_instance->gameClearScene = Scene_GameClear_Create(_instance);

	DodgeGameInstance_ChangeScene(_instance, EST_TITLE);
	return _instance;
}

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->gameInstance != NULL)
		GameInstance_Release(_dodgeGame->gameInstance);

	if (_dodgeGame->titleScene != NULL)
		Scene_Title_Release(_dodgeGame->titleScene);

	if (_dodgeGame->gameScene != NULL)
		Scene_Game_Release(_dodgeGame->gameScene);

	if (_dodgeGame->gameOverScene != NULL)
		Scene_GameOver_Release(_dodgeGame->gameOverScene);

	if (_dodgeGame->gameClearScene != NULL)
		Scene_GameClear_Release(_dodgeGame->gameClearScene);

	free(_dodgeGame);
}

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	float _deltaTime = -1;

	GameInstance_PreTick(_dodgeGame->gameInstance, &_deltaTime);

	switch (_dodgeGame->sceneType)
	{
	case EST_TITLE:
		Scene_Title_Tick(_dodgeGame->titleScene, _dodgeGame, _deltaTime);
		break;
	case EST_GAME:
		Scene_Game_Tick(_dodgeGame->gameScene, _dodgeGame, _deltaTime);
		break;
	case EST_GAMEOVER:
		Scene_GameOver_Tick(_dodgeGame->gameOverScene, _dodgeGame, _deltaTime);
		break;
	case EST_GAMECLAER:
		Scene_GameClear_Tick(_dodgeGame->gameClearScene, _dodgeGame, _deltaTime);
	}

	GameInstance_PostTick(_dodgeGame->gameInstance);
}

void DodgeGameInstance_ChangeScene(DodgeGameInstance* _dodgeGame, ESceneType _targetScene)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->sceneType == _targetScene)
		return;

	_DodgeGameInstance_EndScene(_dodgeGame, _dodgeGame->sceneType);

	_dodgeGame->sceneType = _targetScene;
	_DodgeGameInstance_StartScene(_dodgeGame, _targetScene);
}

void _DodgeGameInstance_StartScene(DodgeGameInstance* _dodgeGame, ESceneType _sceneType)
{
	if (_dodgeGame == NULL)
		return;

	switch (_sceneType)
	{
	case EST_TITLE:
		Scene_Title_OnEnter(_dodgeGame->titleScene, _dodgeGame);
		break;
	case EST_GAME:
		Scene_Game_OnEnter(_dodgeGame->gameScene, _dodgeGame);
		break;
	case EST_GAMEOVER:
		Scene_GameOver_OnEnter(_dodgeGame->gameOverScene, _dodgeGame);
		break;
	case EST_GAMECLAER:
		Scene_GameClear_OnEnter(_dodgeGame->gameClearScene, _dodgeGame);
		break;
	}
}

void _DodgeGameInstance_EndScene(DodgeGameInstance* _dodgeGame, ESceneType _sceneType)
{
	if (_dodgeGame == NULL)
		return;

	switch (_sceneType)
	{
	case EST_TITLE:
		Scene_Title_OnExit(_dodgeGame->titleScene, _dodgeGame);
		break;
	case EST_GAME:
		Scene_Game_OnExit(_dodgeGame->gameScene, _dodgeGame);
		break;
	case EST_GAMEOVER:
		Scene_GameOver_OnExit(_dodgeGame->gameOverScene, _dodgeGame);
		break;
	case EST_GAMECLAER:
		Scene_GameClear_OnExit(_dodgeGame->gameClearScene, _dodgeGame);
		break;
	}
}
