#include "TransitionOnEnd.h"
#include "Sound.h"
TransitionOnEnd::TransitionOnEnd(Sound * _stop, Sound * _start)
{
	sound = _stop;
	secondarySound = _start;
	commandType = CMDTYPES::BUFFER_END;
	instant_execute = true;
}

TransitionOnEnd::TransitionOnEnd()
{
	commandType = CMDTYPES::BUFFER_END;
	instant_execute = true;
}

void TransitionOnEnd::execute()
{
	if (sound && secondarySound)
	{
		sound->Stop();
		secondarySound->Play();
	}
}

TransitionOnEnd * TransitionOnEnd::clone()
{
	return new TransitionOnEnd(*this);
}
