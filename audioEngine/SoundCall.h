#ifndef SoundCall_H
#define SoundCall_H

#include "HandleManager.h"
#include "Handle.h"
#include "snd_err.h"
#include "SoundCallRegistry.h"
#include "SndExecutioner.h"

class SoundCall
{
public:
	// defaults
	SoundCall() = delete;
	SoundCall(const SoundCall &);
	const SoundCall &operator=(const SoundCall &);
	~SoundCall();

	// public
	//SoundCall(const char* SoundCall);
	SoundCall(SoundCallID _id);

	snd_err Play();
	snd_err Pan(float p);
	snd_err Pitch(float p);
	snd_err Volume(float v);
	snd_err Stop();
	snd_err Pause();
	/*
	
	todo -- add filter commands for the submix voice
	
	*/
	snd_err SubmitSoundEndedCallback(CallbackCommand* cmd);

private:
	friend class SoundEngine;
	unsigned int GetHandleNo();

	friend class CallbackSoundEndedCommand;
	snd_err SoundEnded();
	SoundExecutioner* soundEndedBroker;

	SoundCallID id;
	Handle  handle;
};

#endif
