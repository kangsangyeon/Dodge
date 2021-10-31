#include "AudioClip.h"

#include <stdlib.h>
#include <fmod.h>

#include "Audio.h"
#include "Helper.h"

AudioClip* AudioClip_LoadFromFile(Audio* _audio, wchar_t* _filePath, bool _loop)
{
	if (_audio == NULL || _audio->system == NULL)
		return NULL;

	if (_filePath == NULL)
		return NULL;

	FMOD_SOUND* _sound = NULL;

	const char* _filePathMbs = WcharStringToCharString(_filePath);
	const FMOD_MODE _mode = FMOD_2D | (_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	const FMOD_RESULT _result = FMOD_System_CreateSound(_audio->system, _filePathMbs, FMOD_DEFAULT, NULL, &_sound);

	if (_result != FMOD_OK)
		return NULL;

	AudioClip* _outClip = (AudioClip*)malloc(sizeof(AudioClip));
	_outClip->filePath = _filePathMbs;
	_outClip->loop = _loop;
	_outClip->sound = _sound;
	_outClip->channel = NULL;

	return _outClip;
}

void AudioClip_Release(AudioClip* _audioClip)
{
	if (_audioClip == NULL)
		return;

	if (_audioClip->sound != NULL)
		FMOD_Sound_Release(_audioClip->sound);

	if (_audioClip->filePath != NULL)
		free(_audioClip->filePath);

	free(_audioClip);
}

bool AudioClip_IsPlaying(AudioClip* _audioClip)
{
	if (_audioClip == NULL || _audioClip->channel == NULL)
		return false;

	FMOD_BOOL _isPlaying;
	const FMOD_RESULT _result = FMOD_Channel_IsPlaying(_audioClip->channel, &_isPlaying);

	if (_result != FMOD_OK)
		return false;

	return _isPlaying == true;
}
