#pragma once
#include <wchar.h>

#include "Collider.h"
#include "GameInstance.h"

typedef struct TGameInstance GameInstance;
typedef struct TScene_Title Scene_Title;
typedef struct TScene_Game Scene_Game;
typedef struct TScene_GameOver Scene_GameOver;
typedef struct TScene_GameClear Scene_GameClear;

typedef enum TESceneType
{
	EST_NONE = 0,
	EST_TITLE = 1,
	EST_GAME = 2,
	EST_GAMEOVER = 3,
	EST_GAMECLAER = 4
} ESceneType;

typedef struct TDodgeGameInstance
{
	GameInstance* gameInstance;

	ESceneType sceneType;
	Scene_Title* titleScene;
	Scene_Game* gameScene;
	Scene_GameOver* gameOverScene;
	Scene_GameClear* gameClearScene;
} DodgeGameInstance;

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps);

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame);

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame);

void DodgeGameInstance_ChangeScene(DodgeGameInstance* _dodgeGame, ESceneType _targetScene);

void _DodgeGameInstance_StartScene(DodgeGameInstance* _dodgeGame, ESceneType _sceneType);

void _DodgeGameInstance_EndScene(DodgeGameInstance* _dodgeGame, ESceneType _sceneType);
