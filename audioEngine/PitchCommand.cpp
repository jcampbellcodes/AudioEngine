#include "PitchCommand.h"
#include "SoundManager.h"
#include "sndmathtools.h"

PitchCommand::PitchCommand(Sound * snd, float from, float to, float t) : AudioCommand(snd), PitchFrom(from), PitchTo(to), time(t)
{
}

PitchCommand::PitchCommand(float from, float to, float t) : PitchFrom(from), PitchTo(to), time(t)
{
}

void PitchCommand::execute()
{
	if (sound)
	{
		if (!nearEqual(PitchFrom, PitchTo))
		{
			lerpFinished = false;
			//PitchFrom = (1 - time)*PitchFrom + time*PitchTo;
			PitchFrom = PitchFrom + time*(PitchTo - PitchFrom);

			//SoundManager::InsertTimelineCommand(this, 0); // no allocation
		}
		else
		{
			PitchFrom = PitchTo;
			lerpFinished = true;
		}
		//DebugMsg::out("Pitchfrom:%f\n", PitchFrom);
		sound->Pitch(PitchFrom);
	}
}

PitchCommand * PitchCommand::clone()
{
	return new PitchCommand(*this);
}

