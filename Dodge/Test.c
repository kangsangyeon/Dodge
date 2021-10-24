#include "Test.h"

void Test_Screen_RandomizeColor(Screen* _screen)
{
	for (int _y = 0; _y < _screen->height; ++_y)
	{
		for (int _x = 0; _x < _screen->width; ++_x)
		{
			const unsigned short _randomForegroundColor = (rand() + _x + _y) % 0x10;
			const unsigned short _randomBackgroundColor = (rand() + _x + _y + 5) % 0x10;
			const unsigned short _randomColor = _randomForegroundColor | _randomBackgroundColor << 4;

			_screen->colorBuffer[_y][_x] = _randomColor;
		}
	}
}
