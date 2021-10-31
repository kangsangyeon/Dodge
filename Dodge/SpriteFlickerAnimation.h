#pragma once
#include <stdbool.h>

typedef struct TSpriteFlickerAnimation
{
	double flickerRate;

	bool enable;
	bool visible;
	double startTime;
	double lastTransitionTime;
} SpriteFlickerAnimation;

SpriteFlickerAnimation* SpriteFlickerAnimation_Create(double _flickerRate);

void SpriteFlickerAnimation_Release(SpriteFlickerAnimation* _animation);

void SpriteFlickerAnimation_SetEnable(SpriteFlickerAnimation* _animation, bool _newEnable, double _gameTime);

bool SpriteFlickerAnimation_Tick(SpriteFlickerAnimation* _animation, double _gameTime);
