#pragma once

/*
 * Console Handle의 Attribute를 설정하여
 * 콘솔 창에 표시되는 텍스트와 배경의 색상을 설정할 때 사용하는 색상 값입니다.
 * https://stackoverflow.com/questions/6460932/change-entire-console-background-color-win32-c
 */
#define HANDLE_COLOR_BLACK			0x0
#define HANDLE_COLOR_BLUE			0x1
#define HANDLE_COLOR_GREEN			0x2
#define HANDLE_COLOR_AQUA			0x3
#define HANDLE_COLOR_RED			0x4
#define HANDLE_COLOR_PURPLE			0x5
#define HANDLE_COLOR_YELLOW			0x6
#define HANDLE_COLOR_WHITE			0x7
#define HANDLE_COLOR_GRAY			0x8
#define HANDLE_COLOR_LIGHT_BLUE		0x9
#define HANDLE_COLOR_LIGHT_GREEN	0xA
#define HANDLE_COLOR_LIGHT_AQUA		0xB
#define HANDLE_COLOR_LIGHT_RED		0xC
#define HANDLE_COLOR_LIGHT_PURPLE	0xD
#define HANDLE_COLOR_LIGHT_YELLOW	0xE
#define HANDLE_COLOR_BRIGHT_WHITE	0xF

/// <summary>
/// 엔진에서 사용되는 Epsilon 값입니다.
///	이 수치를 변경하여 허용 오차를 조절할 수 있습니다.
/// </summary>
extern const float EPSILON;
