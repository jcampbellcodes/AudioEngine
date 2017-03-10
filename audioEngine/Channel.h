#pragma once
#ifndef Channel_H
#define Channel_H
#include "snd_err.h"
#include "AzulCore.h"

#include "Handle.h"
#include "HandleManager.h"
#include "VoiceFactory.h"


#include <xaudio2.h>
#include <stdio.h>
#include <string>

class CallbackCommand;
class Sound;

class Channel
{
public:
	//big four
	Channel() = delete;
	//Channel(WAVEFORMATEXTENSIBLE _wfx, XAUDIO2_BUFFER* _buffer, Voice* _sourceVoice);
	Channel(IXAudio2SubmixVoice* submix);

	Channel(const Channel& in) = default;
	Channel& operator=(const Channel& in) = default;
	~Channel();

	// public API
	snd_err Pan(float pan);
	snd_err Volume(float volume);




private:
	friend class Sound;
	IXAudio2SubmixVoice* GetSubmixVoice();
	IXAudio2SubmixVoice* voice;

};
#endif