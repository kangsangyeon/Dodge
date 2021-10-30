#pragma once
#include <wchar.h>

#include "Collider.h"
#include "GameInstance.h"

typedef struct TGameInstance GameInstance;
typedef struct TScene_Game Scene_Game;

typedef enum TESceneType
{
	EST_NONE = 0,
	EST_GAME = 1
} ESceneType;

typedef struct TDodgeGameInstance
{
	GameInstance* gameInstance;

	ESceneType sceneType;
	Scene_Game* gameScene;
} DodgeGameInstance;

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, bool _useColor,
                                            int _boardWidth, int _boardHeight, int _desiredFps);

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame);

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame);

void _DodgeGameInstance_GameTick(DodgeGameInstance* _dodgeGame, float _deltaTime);
