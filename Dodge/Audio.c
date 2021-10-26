#include "Audio.h"

#include <stdlib.h>
#include <fmod.h>


Audio* Audio_Create(int _maxChannelCount)
{
	FMOD_SYSTEM* _system = NULL;
	FMOD_RESULT _result = FMOD_System_Create(&_system, FMOD_VERSION);

	if (_result != FMOD_OK)
		return NULL;

	_result = FMOD_System_Init(_system, _maxChannelCount, FMOD_INIT_NORMAL, NULL);
	if (_result != FMOD_OK)
		return NULL;

	Audio* _outAudio = (Audio*)malloc(sizeof(Audio));
	_outAudio->system = _system;

	return _outAudio;
}

void Audio_Release(Audio* _audio)
{
	if (_audio == NULL)
		return;

	if (_audio->system != NULL)
		FMOD_System_Release(_audio->system);

	free(_audio);
}

bool Audio_IsPlaying(AudioClip* _audioClip)
{
	if (_audioClip == NULL || _audioClip->channel == NULL)
		return false;

	FMOD_BOOL _isPlaying;
	const FMOD_RESULT _result = FMOD_Channel_IsPlaying(_audioClip->channel, &_isPlaying);

	if (_result != FMOD_OK)
		return false;

	return _isPlaying == true;
}

bool Audio_Play(Audio* _audio, AudioClip* _audioClip, bool _forcePlay)
{
	if (_audio == NULL || _audio->system == NULL)
		return false;

	if (_audioClip == NULL || _audioClip->sound == NULL)
		return false;

	if (_forcePlay == false && Audio_IsPlaying(_audioClip) == true)
		return false;

	const FMOD_RESULT _result = FMOD_System_PlaySound(_audio->system, _audioClip->sound, NULL, false, &_audioClip->channel);

	if (_result != FMOD_OK)
		return false;

	return true;
}

bool Audio_Stop(AudioClip* _audioClip)
{
	if (_audioClip == NULL || _audioClip->sound == NULL)
		return false;

	if (Audio_IsPlaying(_audioClip) == false)
		return false;

	const FMOD_RESULT _result = FMOD_Channel_Stop(_audioClip->channel);

	if (_result != FMOD_OK)
		return false;

	return true;
}
