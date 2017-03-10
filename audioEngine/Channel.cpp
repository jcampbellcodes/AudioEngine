#include "Channel.h"
#include "SoundManager.h"

Channel::Channel(IXAudio2SubmixVoice * submix) : voice(submix)
{
}

IXAudio2SubmixVoice * Channel::GetSubmixVoice()
{
	return voice;
}

Channel::~Channel()
{
	voice->DestroyVoice();
}

snd_err Channel::Pan(float pan)
{
	// generate matrix
	float mat[8] = { 0 };
	// calculate peker values based on pan
	float left = 0.5f - pan / 2.0f;
	float right = 0.5f + pan / 2.0f;
	mat[0] = left;
	mat[1] = right;

	// send to voice
	SoundManagerInfo info;
	SoundManager::GetInfo(info);
	XAUDIO2_VOICE_DETAILS masteringVoiceDetails = info.voiceDetails;
	XAUDIO2_VOICE_DETAILS sourceVoiceDetails;
	voice->GetVoiceDetails(&sourceVoiceDetails);

	voice->SetOutputMatrix(nullptr, sourceVoiceDetails.InputChannels, masteringVoiceDetails.InputChannels, mat);

	return snd_err::OK;
}

snd_err Channel::Volume(float volume)
{

	voice->SetVolume(volume);
	return snd_err::OK;
}