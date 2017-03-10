#ifndef SND_MANAGER_H
#define SND_MANAGER_H

#define _USE_TRACE_NEW

#if defined(_DEBUG) && defined(_USE_TRACE_NEW)
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif


#include "CircularData.h"
#include "AudioCommand.h"
#include "snd_err.h"
#include "SoundCall.h"
#include "SoundCallRegistry.h"

#include <map>

class SoundEngine
{
public:
	// Defaults
	SoundEngine(const SoundEngine &) = delete;
	const SoundEngine &operator = (const SoundEngine &) = delete;


	// Public methods:
	static snd_err Create()
	{
		return Instance().privCreate();
	}
	static snd_err Update()
	{
		return Instance().privUpdate();
	}
	static snd_err SendData(ThreadCommand val)
	{
		return Instance().privSendData(val);
	}

	static snd_err SendDataA2G(ThreadCommand val)
	{
		return Instance().privSendDataA2G(val);
	}

	static CircularData *GetG2ABuff()
	{
		return Instance().privGetG2ABuff();
	}
	static CircularData *GetA2GBuff()
	{
		return Instance().privGetA2GBuff();
	}


	// Manager pool related
	static snd_err CreateSoundCall(SoundCall*&in, SoundCallID id)
	{
		return Instance().privCreateSoundCall(in, id);
	}


	static snd_err ReturnSoundCall(SoundCall* in)
	{
		return Instance().privReturnSoundCall(in);
	}

	static snd_err FindSoundCall(SoundCall*& out, unsigned int handle)
	{
		return Instance().privFindSoundCall(out, handle);
	}

	static snd_err Terminate()
	{
		return Instance().privTerminate();
	}

private:
	static SoundEngine *instance;
	static SoundEngine& Instance()
	{
		if (!instance)
		{
			instance = new SoundEngine();
		}
		return *instance;
	}

	SoundEngine();
	~SoundEngine(); // probably should die

	CircularData *pG2ABuff;  // Game to Audio
	CircularData *pA2GBuff; // Audio to Game

	// private methods
	snd_err privUpdate();
	snd_err privSendData(ThreadCommand val);
	snd_err privSendDataA2G(ThreadCommand val);
	snd_err privCreate();
	snd_err privTerminate();

	// manager pool
	snd_err privCreateSoundCall(SoundCall*&in, SoundCallID id);
	snd_err privReturnSoundCall(SoundCall* in); // do we need it?
	snd_err privFindSoundCall(SoundCall*& out, unsigned int handle);

	std::map<unsigned int, SoundCall*> activeSoundCalls;

	CircularData *privGetG2ABuff();
	CircularData *privGetA2GBuff();

	std::thread audioThread;


};
#endif
