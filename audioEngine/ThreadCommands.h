#pragma once
#ifndef THREADCMDS_H
#define THREADCMDS_H

#include "ThreadCommunicationCommand.h"
#include "StartPlaylistCommand.h"
#include "CallbackSoundEndedCommand.h"
#include "TerminateSoundEngineCommand.h"

/*
union ThreadCommand
{
	StartPlaylistCommand startcmd;
	CallbackSoundEndedCommand callbackendedcmd;
	TerminateSoundEngineCommand terminateengine;
};
*/

struct ThreadCommand
{
	ThreadCommandType type;
	unsigned int handle;
	SoundCallID id;
	float pan;
	float pitch;
	float volume;
};

#endif