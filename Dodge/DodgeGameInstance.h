#pragma once
#include <wchar.h>

#include "Board.h"
#include "GameInstance.h"
#include "Player.h"

typedef struct TDodgeGameInstance
{
	GameInstance* gameInstance;
	Board* board;

	int screenWidth;
	int screenHeight;

	// for test
	Player* player;
	WorldObject* paimon;
} DodgeGameInstance;

DodgeGameInstance* DodgeGameInstance_Create(int _screenWidth, int _screenHeight, wchar_t* _fontFaceName, COORD _fontSize,
                                            unsigned short _foregroundColor, unsigned short _backgroundColor, int _boardWidth, int _boardHeight, int _desiredFps);

void DodgeGameInstance_Release(DodgeGameInstance* _dodgeGame);

void DodgeGameInstance_Tick(DodgeGameInstance* _dodgeGame);

void _DodgeGameInstance_GameTick(DodgeGameInstance* _dodgeGame, float _deltaTime);
