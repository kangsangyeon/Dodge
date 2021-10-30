#include "DodgeGameInstance.h"

#include <stdbool.h>
#include <stdlib.h>

#include "Scene_Game.h"

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps)
{
	DodgeGameInstance* _instance = (DodgeGameInstance*)calloc(1, sizeof(DodgeGameInstance));
	_instance->gameInstance = GameInstance_Create(_screenWidth, _screenHeight, _fontFaceName, _fontSize, _foregroundColor, _backgroundColor, _useColor, _desiredFps);

	_instance->sceneType = EST_GAME;
	_instance->gameScene = Scene_Game_Create(_instance);

	return _instance;
}

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	if (_dodgeGame->gameInstance != NULL)
		GameInstance_Release(_dodgeGame->gameInstance);

	if (_dodgeGame->gameScene != NULL)
		Scene_Game_Release(_dodgeGame->gameScene);

	free(_dodgeGame);
}

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame)
{
	if (_dodgeGame == NULL)
		return;

	float _deltaTime = -1;

	GameInstance_PreTick(_dodgeGame->gameInstance, &_deltaTime);

	_DodgeGameInstance_GameTick(_dodgeGame, _deltaTime);

	GameInstance_PostTick(_dodgeGame->gameInstance);
}

void _DodgeGameInstance_GameTick(DodgeGameInstance* _dodgeGame, float _deltaTime)
{
	switch (_dodgeGame->sceneType)
	{
	case EST_GAME:
		Scene_Game_Tick(_dodgeGame->gameScene, _dodgeGame, _deltaTime);
		break;
	}
}
