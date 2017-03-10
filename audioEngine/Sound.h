#pragma once
#ifndef SOUND_H
#define SOUND_H
#include "snd_err.h"
#include "AzulCore.h"

#include "Handle.h"
#include "HandleManager.h"
#include "VoiceFactory.h"


#include <xaudio2.h>
#include <stdio.h>
#include <string>
#include <stack>

class CallbackCommand;
class Channel;

class Sound
{
public:
	//big four
	Sound() = delete; // can't have a sound without a file
	Sound(WAVEFORMATEXTENSIBLE _wfx, XAUDIO2_BUFFER* _buffer);


	Sound(const Sound& in) = default;
	Sound& operator=(const Sound& in) = default;
	~Sound();

	// Sound manager has master voice and XAudio2 COM
	// user just uses these methods on creation of a sound
	
	// gets the sound through an accessor in sound manager
	// returns pointer to this sound
	// calls a method on this sound

	// public API
	snd_err Play();
	snd_err Stop();
	snd_err Pause();
	snd_err Pan(float pan);
	snd_err Pitch(float pitch);
	snd_err Volume(float volume);
	snd_err Loop(bool toLoop);

	snd_err SubmitCallback(CallbackCommand* cmd);

	snd_err SendToTrack(int priority);
	snd_err AddToChannel(Channel* channel);

	snd_err ClearTrack();

	bool IsPlaying();

	unsigned int LengthInBytes();

private:

	//playback
	Track* track;
	XAUDIO2_BUFFER buffer;
	WAVEFORMATEXTENSIBLE wfx;

	unsigned int lengthInBytes;

	bool playing;

	int startPosition;

	std::stack<CallbackCommand*> cmdStack;

	friend class SoundManager;

};







#endif