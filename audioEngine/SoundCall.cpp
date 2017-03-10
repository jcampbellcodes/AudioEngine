#include <assert.h>

#include "CircularData.h"
#include "SoundEngine.h"
#include "SoundCall.h"
#include "HashThis.h"
#include "StartPlaylistCommand.h"

/*
SoundCall::SoundCall(const char* SoundCall)
: handle()
{
	this->id = HashThis(SoundCall);
}
*/

SoundCall::SoundCall(SoundCallID _id) : id(_id)
{
	this->soundEndedBroker = new SoundExecutioner();


	// Send a play command to the Audio thread
	CircularData *pOut = SoundEngine::GetG2ABuff();
	assert(pOut);

	//StartPlaylistCommand* cmd = new StartPlaylistCommand();
	//cmd->SetContext(this->id, this->handle.GetID());
	ThreadCommand cmd;
	cmd.handle = this->handle.GetID();
	cmd.id = this->id;
	cmd.type = ThreadCommandType::CreatePlaylist;
	pOut->PushBack(cmd);


}

SoundCall::SoundCall(const SoundCall &tmp)
: id(tmp.id),
  handle()
{
}

const SoundCall &SoundCall::operator=(const SoundCall &tmp)
{
	this->id = tmp.id;

	return *this;
}

SoundCall::~SoundCall()
{  
	if (soundEndedBroker)
	{
		delete soundEndedBroker;
		soundEndedBroker = nullptr;
	}
	HandleManager::DeregisterHandle(handle.GetID(), this->handle.GetIndex());
}


snd_err SoundCall::Play()
{
	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.type = ThreadCommandType::Play;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::Pan(float p)
{
	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.pan = p;
		cmd.type = ThreadCommandType::Pan;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::Pitch(float p)
{
	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.pitch = p;
		cmd.type = ThreadCommandType::Pitch;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::Volume(float v)
{
	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.volume = v;
		cmd.type = ThreadCommandType::Volume;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::Stop()
{
	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.type = ThreadCommandType::Stop;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::Pause()
{

	snd_err err = snd_err::OK;
	if (HandleManager::CheckID(handle) == snd_err::OK)
	{
		// Send a play command to the Audio thread
		CircularData *pOut = SoundEngine::GetG2ABuff();
		assert(pOut);

		//StartPlaylistCommand* cmd = new StartPlaylistCommand();
		//cmd->SetContext(this->id, this->handle.GetID());
		ThreadCommand cmd;
		cmd.handle = this->handle.GetID();
		cmd.id = this->id;
		cmd.type = ThreadCommandType::Pause;
		pOut->PushBack(cmd);

	}
	else
	{
		return snd_err::ERR;
	}
	return err;
}

snd_err SoundCall::SubmitSoundEndedCallback(CallbackCommand * cmd)
{
	snd_err err = snd_err::OK;

	this->soundEndedBroker->addCommand(cmd);

	return err;
}

unsigned int SoundCall::GetHandleNo()
{
	return handle.GetID();
}

// internal callback
snd_err SoundCall::SoundEnded()
{
	snd_err err = snd_err::OK;

	this->soundEndedBroker->ProcessInstantCommands();

	return err;
}
