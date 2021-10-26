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
