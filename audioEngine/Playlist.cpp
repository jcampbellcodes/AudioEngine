#include "Playlist.h"
#include "SoundManager.h"
#include "Sound.h"
#include "CallbackSoundEndedCommand.h"
#include "HashThis.h"
#include "CallbackCommand.h"
#include "PlayCmd.h"
#include "PanCmd.h"
#include "LoopCommand.h"
#include "PitchCommand.h"
#include "VolumeCommand.h"
#include "SoundManager.h"
#include "AudioMain.h"
#include "TransitionOnEnd.h"
#include "AssignCallbackCommand.h"
#include "InternalSoundEnded.h"


#include "SoundEngine.h"

Playlist::Playlist(SoundCallID _id, unsigned int instance) : id(_id), soundsLoaded(false), instanceNumber(instance)
{
	SoundManager::CreateChannel(channel);
	assert(channel);
}

CmdWithTime::~CmdWithTime()
{
	if (cmd)
	{
		//delete cmd;
		cmd = nullptr;
	}
}

Playlist::~Playlist()
{
	// CLEAN OUT PLAYLIST LISTS + MAP




	// clean list of cmds
	std::list<CmdWithTime*>::iterator cmdIter = commands.begin();
	while (cmdIter != commands.end())
	{
		delete (*cmdIter);
		(*cmdIter) = nullptr;
		cmdIter++;
	}
	commands.clear();

	// clean map of sounds
	std::map<unsigned int, Sound*>::iterator sndIter = sounds.begin();
	while (sndIter != sounds.end())
	{
		delete (*sndIter).second;
		sndIter++;
	}

	if (channel)
	{
		delete channel;
	}

}

snd_err Playlist::PrepareToPlay()
{
	snd_err err = snd_err::OK;
	// grab voices for all the sounds
	// put sounds in a channel
	Sound* longestSound = nullptr;
	unsigned int longestSoundLength = 0;
	std::map<unsigned int, Sound*>::iterator iter = sounds.begin();
	while (iter != sounds.end())
	{
		err = (*iter).second->SendToTrack(0);
		if (!longestSound || (*iter).second->LengthInBytes() > longestSoundLength)
		{
			longestSound = (*iter).second;
		}
		if (err == snd_err::NO_TRACK) // loses priority!
		{
			break;
		}
		err = (*iter).second->AddToChannel(channel);
		iter++;
	}

	longestSound->SubmitCallback(new InternalSoundEnded(this));
	// all geared up!

	soundsLoaded = true;

	return err;
}
/*
snd_err Playlist::AddSound(Sound * snd)
{
	snd_err err = snd_err::OK;
	if (snd)
	{
		this->sounds.push_back(snd);
	}
	else
	{
		err = snd_err::ERR;
	}
	return err;
}
*/

snd_err Playlist::AddSound(const char * sndName)
{
	snd_err err = snd_err::OK;
	if (sndName)
	{
		Sound* snd = nullptr;
		unsigned int sndHash = HashThis(sndName);

		err = SoundManager::GetSound(snd, sndHash);

		if (err == snd_err::OK)
		{
			this->sounds.insert(std::pair<unsigned int, Sound*>(sndHash, snd));
		}
	}
	else
	{
		err = snd_err::ERR;
	}
	return err;
}

snd_err Playlist::AddSound(unsigned int sndName)
{
	snd_err err = snd_err::OK;
	if (sndName)
	{
		Sound* snd = nullptr;
		unsigned int sndHash = sndName;

		err = SoundManager::GetSound(snd, sndHash);

		if (err == snd_err::OK)
		{
			this->sounds.insert(std::pair<unsigned int, Sound*>(sndHash, snd));
		}
	}
	else
	{
		err = snd_err::ERR;
	}
	return err;
}


snd_err Playlist::AddCommand(AudioCommand * cmd, int time, unsigned int _id)
{
	snd_err err = snd_err::OK;
	cmd->AttachSound(this->sounds.at(_id));
	cmd->AssignPlaylistID(instanceNumber);

	CmdWithTime* command = new CmdWithTime();
	command->cmd = cmd;
	command->cmd->AssignPlaylistID(instanceNumber);
	command->time = time;

	if (cmd)
	{
		this->commands.push_back(command);
	}
	else
	{
		err = snd_err::ERR;
	}
	return err;
}

snd_err Playlist::AddCommand(AudioCommand * cmd, int time)
{
	snd_err err = snd_err::OK;
	cmd->AssignPlaylistID(instanceNumber);

	CmdWithTime* command = new CmdWithTime();
	command->cmd = cmd;
	command->cmd->AssignPlaylistID(instanceNumber);
	command->time = time;

	if (cmd)
	{
		this->commands.push_back(command);
	}
	else
	{
		err = snd_err::ERR;
	}
	return err;
}

// special for callbacks
snd_err Playlist::AddCommand(CallbackCommand * cmd, int time, unsigned int id1, unsigned int id2 = 0)
{
	snd_err err = snd_err::OK;
	cmd->AttachSound(this->sounds.at(id1));
	cmd->AssignPlaylistID(instanceNumber);
	if (id2)
	{
		cmd->AttachSecondarySound(this->sounds.at(id2));
	}
	CmdWithTime* command = new CmdWithTime();
	command->cmd = new AssignCallbackCommand(cmd);
	command->cmd->AttachSound(this->sounds.at(id1));
	command->cmd->AssignPlaylistID(instanceNumber);
	command->time = time;

	if (cmd)
	{
		this->commands.push_back(command);
	}
	else
	{
		err = snd_err::ERR;
	}

	return err;
}

snd_err Playlist::SendCallbackSoundEnded()
{
	snd_err err = snd_err::OK;

	//CallbackSoundEndedCommand* cmd = new CallbackSoundEndedCommand(this->id, this->instanceNumber);
	//cmd;
	ThreadCommand cmd;
	cmd.handle = this->instanceNumber;
	cmd.id = this->id;
	cmd.type = ThreadCommandType::CallbackSoundEnded;
	SoundEngine::SendData(cmd);

	return err;
}

snd_err Playlist::Play() // start the commands
{
	snd_err err = snd_err::OK;

	if (!soundsLoaded)
	{
		this->PrepareToPlay();
	}

	std::list<CmdWithTime*>::iterator iter = commands.begin();
	while (iter != commands.end())
	{
		SoundManager::InsertTimelineCommand((*iter)->cmd, (*iter)->time);
		iter++;
	}

	startTime.tic();

	return err;
}

snd_err Playlist::Stop() // stop commands ------- may have to do some thinking here...
{
	snd_err err = snd_err::OK;

	SoundManager::CancelPlaylist(this->instanceNumber);

	std::map<unsigned int, Sound*>::iterator iterSnd = sounds.begin();
	while (iterSnd != sounds.end())
	{
		(*iterSnd).second->Stop();
		iterSnd++;
	}



	// clean list of cmds
	std::list<CmdWithTime*>::iterator cmdIter = commands.begin();
	while (cmdIter != commands.end())
	{
		delete (*cmdIter);
		(*cmdIter) = nullptr;
		cmdIter++;
	}
	commands.clear();

	// clean map of sounds
	std::map<unsigned int, Sound*>::iterator sndIter = sounds.begin();
	while (sndIter != sounds.end())
	{
		delete (*sndIter).second;
		sndIter++;
	}

	if (channel)
	{
		delete channel;
	}


	return err;
}

snd_err Playlist::Pause() // take commands off queue and accumulates offset in case you un-pause
{
	// ooooo mama
	snd_err err = snd_err::OK;

	SoundManager::CancelPlaylist(this->instanceNumber);

	const Time elapsedTime = startTime.toc();
	int stopTime = Time::quotient(elapsedTime, Time(TIME_ONE_MILLISECOND));

	std::list<CmdWithTime*>::iterator iterCmd = commands.begin();
	std::list<CmdWithTime*>::iterator iterCmdDelete = commands.begin();
	int adjustedTime = 0;
	while (iterCmd != commands.end())
	{
		adjustedTime = (*iterCmd)->time - stopTime;
		if (adjustedTime < 0) // command already happened
		{
			iterCmdDelete = iterCmd;
			iterCmd++;
			commands.remove(*iterCmdDelete);
		}
		else // keep the command to use later but adjust time
		{
			
			(*iterCmd)->time = adjustedTime;
			iterCmd++;
		}

	}

	std::map<unsigned int, Sound*>::iterator iterSnd = sounds.begin();
	while (iterSnd != sounds.end())
	{

		if ((*iterSnd).second->IsPlaying())
		{
			(*iterSnd).second->Pause(); // does this work right?
			this->AddCommand(new PlayCommand((*iterSnd).second), 0);
		}
		iterSnd++;
	}

	return err;
}

snd_err Playlist::Pan(float pan) // pan the submix voice
{
	snd_err err = snd_err::OK;
	if (channel)
	{
		this->channel->Pan(pan);
	}
	else
	{
		err = snd_err::NULLPTR;
	}

	return err;
}

snd_err Playlist::Pitch(float pitch) // loop through each sound and change pitch
{
	snd_err err = snd_err::OK;
	std::map<unsigned int, Sound*>::iterator iterSnd = sounds.begin();
	while (iterSnd != sounds.end())
	{
		(*iterSnd).second->Pitch(pitch); // does this work right?
		iterSnd++;
	}
	return err;
}

snd_err Playlist::Volume(float volume) // volume for the submix voice
{
	snd_err err = snd_err::OK;
	if (channel)
	{
		this->channel->Volume(volume);
	}
	else
	{
		err = snd_err::NULLPTR;
	}
	return err;
}

unsigned int Playlist::GetInstanceNumber()
{
	return instanceNumber;
}
