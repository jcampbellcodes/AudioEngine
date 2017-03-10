#pragma once
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "snd_err.h"
#include "SoundCallRegistry.h"
#include "Timer.h"

#include <list>
#include <map>
class Channel;
class Sound;
class AudioCommand;
class CallbackCommand;

struct CmdWithTime
{
	~CmdWithTime();

	int time;
	AudioCommand* cmd;
};

class Playlist
{
public:
	// big four
	Playlist() = delete;
	Playlist(SoundCallID id, unsigned int instance);
	Playlist(const Playlist&);
	Playlist& operator=(const Playlist&);
	~Playlist();

	// occupy all sounds
	snd_err PrepareToPlay();

	// add sound -- doesn't make sense if can't be referred to by script. needs to have an identifier
	//snd_err AddSound(Sound* snd);

	// add sound by name
	snd_err AddSound(const char* snd);

	// add sound by hash
	snd_err AddSound(unsigned int snd);

	// add command
	snd_err AddCommand(AudioCommand* cmd, int time, unsigned int id);


	// add callback command
	snd_err AddCommand(CallbackCommand* cmd, int time, unsigned int id1, unsigned int id2);

	// tell game side we are done
	snd_err SendCallbackSoundEnded();

	// public API
	snd_err Play();
	snd_err Stop();
	snd_err Pause();
	snd_err Pan(float pan);
	snd_err Pitch(float pitch);
	snd_err Volume(float volume);

	unsigned int GetInstanceNumber();

private:

	snd_err AddCommand(AudioCommand * cmd, int time);
	// list of commands to add to timeline
	std::list<CmdWithTime*> commands;
	// list of Sounds to get
	std::map<unsigned int, Sound*> sounds;
	Channel* channel;

	bool soundsLoaded;

	// ID from enum
	SoundCallID id;

	Timer startTime;

	// instance identifier -> soundcall handle hashed
	unsigned int instanceNumber;

};

#endif