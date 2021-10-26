#pragma once
#include <fmod_common.h>

typedef struct TAudio
{
	FMOD_SYSTEM* system;

	int soundCount;
	int channelCount;
	FMOD_SOUND** soundArr;
	FMOD_CHANNEL** channelArr;
} Audio;

Audio* Audio_Create();

void Audio_Release();
