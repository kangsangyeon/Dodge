#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <Windows.h>

#include "Const.h"
#include "DodgeGameInstance.h"
#include "Sprite.h"

/// <summary>
/// 화면의 크기를 정의하는 값입니다.
///	1당 한 글자를 의미합니다. 즉, 높이 200은 세로로 200개의 글자가 표시될 수 있는 크기를 의미합니다.
///	설정 가능한 화면의 최대 크기보다 더 높은 값으로 설정하면 화면 크기 설정이 적용되지 않을 수 있습니다.
/// </summary>
const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 200;

/// <summary>
/// 화면에 출력될 폰트입니다.
/// </summary>
const wchar_t* FONT_FACE_NAME = L"Terminal";

/// <summary>
/// 화면에 출력될 폰트의 크기를 선택하는 값입니다.
///	화면의 최대 크기를 결정하는 요소입니다.
/// </summary>
const int FONT_SIZE_INDEX = 0;

const int BOARD_WIDTH = 40;
const int BOARD_HEIGHT = 40;
const int DESIRED_FPS = 60;

const unsigned short FOREGROUND_COLOR = HANDLE_COLOR_BLACK;
const unsigned short BACKGROUND_COLOR = HANDLE_COLOR_BRIGHT_WHITE;
const bool USE_COLOR = false;

const COORD FONT_SIZE_RASTER[] = {
	{3, 5},
	{4, 6},
	{5, 8},
	{6, 8},
	{8, 8},
	{12, 16}
};


bool exitFlag = false;

Sprite* _sprite;

void onInterrupt(int sig)
{
	exitFlag = true;
}

int main()
{
	signal(SIGINT, onInterrupt);
	setlocale(LC_ALL, "KOREAN");
	SetConsoleTitle(TEXT("THIS GAME IS...? Doge : Dodge"));
	srand(time(NULL));

	DodgeGameInstance* _dodgeGame = DodgeGameInstance_Create(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		FONT_FACE_NAME,
		FONT_SIZE_RASTER[FONT_SIZE_INDEX],
		FOREGROUND_COLOR,
		BACKGROUND_COLOR,
		USE_COLOR,
		BOARD_WIDTH,
		BOARD_HEIGHT,
		DESIRED_FPS);

	do
	{
		DodgeGameInstance_Tick(_dodgeGame);
	}
	while (exitFlag == false);

	Sprite_Release(_sprite);

	DodgeGameInstance_Release(_dodgeGame);
	return 0;
}
