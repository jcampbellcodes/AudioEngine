#pragma once
#ifndef SOUNDCALLREG_H
#define SOUNDCALLREG_H

typedef enum SoundCallID
{
	Seinfeld
} SoundCallID;


typedef enum ThreadCommandType
{
	// stored with their md5 hash values
	Play = 0xee8a5b16,
	Stop,
	Load,
	Pause,
	Pan = 0xc35b1997,
	Pitch = 0xada3d01f,
	Volume = 0x5b339ff6,
	CallbackSoundEnded,
	CallbackSoundLoaded,
	CallbackSoundStarted,
	TerminateSoundEngine,
	CreatePlaylist
} ThreadCommandType;


struct SoundCallCommunication
{
	ThreadCommandType type;
	SoundCallID id;
	unsigned int handle;
	float pan;
	float pitch;
	float volume;
};



#endif