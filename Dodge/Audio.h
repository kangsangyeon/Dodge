#pragma once
#include <fmod_common.h>

typedef struct TAudio
{
	FMOD_SYSTEM* system;
} Audio;

Audio* Audio_Create(int _maxChannelCount);

void Audio_Release(Audio* _audio);
