#pragma once
#include <fmod_common.h>
#include <stdbool.h>
#include <wchar.h>

typedef struct TAudioClip
{
	char* filePath;
	bool loop;

	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
} AudioClip;

AudioClip* AudioClip_LoadFromFile(struct Audio* _audio, wchar_t* _filePath, bool _loop);

void AudioClip_Release(AudioClip* _audioClip);
