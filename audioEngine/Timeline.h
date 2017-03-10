#pragma once
#ifndef Timeline_H
#define Timeline_H

#include <map>
#include "AzulCore.h"
#include "snd_err.h"
#include "Timer.h"

class Alarmable;
class AudioCommand;

class Timeline
{
public:

	// big four
	Timeline();
	~Timeline();
	Timeline(const Timeline&) = delete;
	Timeline& operator=(const Timeline&) = delete;
	

	snd_err Register(AudioCommand* cmd, int t);
	snd_err Deregister(AudioCommand* cmd);

	snd_err RemovePlaylist(unsigned int instance);

	snd_err ProcessAlarms();
	int GetTime();

private:
	Timer timer;
	friend class Alarmable;
	std::multimap<int, AudioCommand*> timeline;

};

#endif