#include <assert.h>

#include "Trace\Trace.h"
#include "SoundEngine.h"
#include "ThreadHelper.h"
#include "AudioMain.h"
#include "SoundManager.h"
#include "TerminateSoundEngineCommand.h"
#include "SoundManager.h"

SoundEngine *SoundEngine::instance = nullptr;

SoundEngine::SoundEngine()
{

}

SoundEngine::~SoundEngine()
{
	

	std::map<unsigned int, SoundCall*>::iterator iter = this->activeSoundCalls.begin();
	while (iter != activeSoundCalls.end())
	{
		delete (*iter).second;
		iter++;
	}
	activeSoundCalls.clear();


	/*
	if (this->pA2GBuff)
	{
		delete pA2GBuff;
	}
	
	if (this->pG2ABuff)
	{
		delete pG2ABuff;
	}
	*/

}

snd_err SoundEngine::privCreateSoundCall(SoundCall *& in, SoundCallID id)
{
	snd_err err = snd_err::OK;

	in = new SoundCall(id);

	this->activeSoundCalls.insert(std::pair<unsigned int, SoundCall*>(in->GetHandleNo(), in));

	return err;
}

snd_err SoundEngine::privFindSoundCall(SoundCall*& out, unsigned int handle)
{
	snd_err err = snd_err::OK;

	out = this->activeSoundCalls.at(handle);

	return err;
}

CircularData * SoundEngine::privGetG2ABuff()
{
	return this->pG2ABuff;
}

CircularData * SoundEngine::privGetA2GBuff()
{
	return this->pA2GBuff;
}

snd_err SoundEngine::privSendData(ThreadCommand data)
{
	snd_err err = snd_err::OK;
	//Trace::out("Game->Audio: %x \n", data);
	this->pG2ABuff->PushBack(data);
	return err;
}

snd_err SoundEngine::privSendDataA2G(ThreadCommand data)
{
	snd_err err = snd_err::OK;
	//Trace::out("Audio->Game: %x \n", data);
	this->pA2GBuff->PushBack(data);
	return err;
}

snd_err SoundEngine::privUpdate()
{
	snd_err err = snd_err::OK;
	ThreadCommand val;

	if (this->pA2GBuff->PopFront(val) == true)
	{
		//val->execute();
	}
	return err;
}


snd_err SoundEngine::privCreate()
{
	snd_err err = snd_err::OK;
	this->pG2ABuff = new CircularData();
	assert(pG2ABuff);

	this->pA2GBuff = new CircularData();
	assert(pA2GBuff);

	SetMainThreadName("GAME");
	audioThread = std::thread(AudioMain, this->pG2ABuff, this->pA2GBuff);
	SetThreadName(audioThread, "AUDIO");
	
	audioThread.detach();


	return err;
}


snd_err SoundEngine::privTerminate()
{
	//SoundManager::Unload();
	ThreadCommand cmd;
	cmd.type = ThreadCommandType::TerminateSoundEngine;
	this->privSendData(cmd);

	
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
	


	return snd_err::OK;
}
