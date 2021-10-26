#include "Audio.h"

#include <stdlib.h>
#include <fmod.h>


Audio* Audio_Create()
{
	FMOD_SYSTEM* _system = NULL;
	FMOD_RESULT _result = FMOD_System_Create(&_system, FMOD_VERSION);

	if(_result != FMOD_OK)
		return NULL;

	//FMOD_System_CreateSound()


	Audio* _outAudio = (Audio*)malloc(sizeof(Audio));

	return _outAudio;
}
