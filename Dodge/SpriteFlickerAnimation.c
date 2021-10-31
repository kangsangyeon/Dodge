#include "SpriteFlickerAnimation.h"

#include <stdlib.h>


SpriteFlickerAnimation* SpriteFlickerAnimation_Create(double _flickerRate)
{
	SpriteFlickerAnimation* _outAnimation = (SpriteFlickerAnimation*)malloc(sizeof(SpriteFlickerAnimation));
	_outAnimation->enable = false;
	_outAnimation->startTime = 0.;

	_outAnimation->flickerRate = _flickerRate;

	return _outAnimation;
}

void SpriteFlickerAnimation_Release(SpriteFlickerAnimation* _animation)
{
	if (_animation == NULL)
		return;

	free(_animation);
}

void SpriteFlickerAnimation_SetEnable(SpriteFlickerAnimation* _animation, bool _newEnable, double _gameTime)
{
	if (_animation == NULL)
		return;

	if (_animation->enable == _newEnable)
		return;

	_animation->enable = _newEnable;

	if (_animation->enable)
	{
		_animation->startTime = _gameTime;
		_animation->lastTransitionTime = _gameTime;
		_animation->visible = false;
	}
	else
	{
		_animation->startTime = 0;
		_animation->lastTransitionTime = 0;
		_animation->visible = true;
	}
}

bool SpriteFlickerAnimation_Tick(SpriteFlickerAnimation* _animation, double _gameTime)
{
	if (_animation == NULL)
		return false;

	if (_animation->enable == false)
		return false;

	const double _elapsedTimeAfterLastTransition = _gameTime - _animation->lastTransitionTime;
	const bool _shouldFlicker = _elapsedTimeAfterLastTransition >= _animation->flickerRate;

	if (_shouldFlicker == true)
	{
		_animation->visible = !_animation->visible;
		_animation->lastTransitionTime = _gameTime;
	}

	return _animation->visible;
}
