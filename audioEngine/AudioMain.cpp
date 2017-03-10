#include <thread>
#include <assert.h>

#include "Trace/Trace.h"
#include "File.h"
#include "PlayCmd.h"
#include "PanCmd.h"
#include "LoopCommand.h"
#include "PitchCommand.h"
#include "VolumeCommand.h"
#include "SoundManager.h"
#include "AudioMain.h"
#include "TransitionOnEnd.h"
#include "Playlist.h"

#include "PlaylistManager.h"

#include "ThreadCommunicationCommand.h"
#include "StartPlaylistCommand.h"

#include "SoundCall.h"

#include "SoundEngine.h"

void AudioMain(CircularData *pIn, CircularData *pOut)
{
	pOut;
	/*
	
	
	TODO!
	
	1) move the priority to the game thread. lifetime + priority numbers so a sound knows INSTANTLY if it can play or not.

	2) load sounds asynchronously
	-- on game side, say "load script", give it a path and ID. the user will use that script ID to do their stuff.
	-- audiomain gets the command with that info, does e.g., PlaylistManager::InitializePlaylistDescriptor("SeinfeldScript.txt", SoundCallID::Seinfeld);
	---- change buffer manager so its data is buffer pointers that are initialized as null
	-- playlist manager sends the file load request to the file thread

	3) update instancing so soundcalls will play once and be killed at the end. call the on_end callbacks and then after that invalidate the handle and then send a message to invalidate the audio thread sound
	
	
	*/
	// this will be done by a loading script ---- probably will all be changed when we devise and tackle a soundbank solution
	SoundManager::LoadSound("a", "A.wav");
	SoundManager::LoadSound("trans", "AtoB.wav");
	SoundManager::LoadSound("b", "B.wav");
	SoundManager::LoadSound("applause", "applause-8.wav");
	PlaylistManager::InitializePlaylistDescriptor("SeinfeldScript.txt", SoundCallID::Seinfeld);


	/*


	priority test example -- on the AUDIO THREAD


	*/

	int i = 0;
	snd_err err = snd_err::OK;
	while (i < DEFAULT_NUM_VOICES)
	{
		Sound* applause = nullptr;
		SoundManager::GetSound(applause, "applause");
		err = applause->SendToTrack(100);
		i++;
	}
	err;
	Sound* applause = nullptr;
	SoundManager::GetSound(applause, "applause");
	err = applause->SendToTrack(200);
	err = applause->Play();

	/*
	
	
	end priority test example
	
	
	*/

	bool ended = false;
	
    while (!ended)
	{
		SoundManager::Update();
		
		ThreadCommand cmd;
		
		if (pIn->PopFront(cmd) == true) // there is something there!!!! wooooooo
		{
			switch (cmd.type)
			{
			case ThreadCommandType::Play:
			{
				Playlist* playlist = nullptr;
				if (PlaylistManager::GetPlaylist(playlist, cmd.handle) == snd_err::OK)
				{
					playlist->Play();
				}
				break;
			}
			case ThreadCommandType::CreatePlaylist:
			{
				PlaylistManager::StartPlaylist(cmd.id, cmd.handle);
				break;
			}
			case ThreadCommandType::CallbackSoundEnded:
			{
				CallbackSoundEndedCommand endedcb;
				endedcb.SetContext(cmd.id, cmd.handle);
				endedcb.execute();
				break;
			}
			case ThreadCommandType::TerminateSoundEngine:
			{
				ended = true;
				break;
			}
			case ThreadCommandType::Stop:
			{
				PlaylistManager::StopPlaylist(cmd.handle);
				break;
			}
			case ThreadCommandType::Pause:
			{
				Playlist* playlist = nullptr;
				if (PlaylistManager::GetPlaylist(playlist, cmd.handle) == snd_err::OK)
				{
					playlist->Pause();
				}
				break;
			}
			case ThreadCommandType::Pan:
			{
				Playlist* playlist = nullptr;
				if (PlaylistManager::GetPlaylist(playlist, cmd.handle) == snd_err::OK)
				{
					playlist->Pan(cmd.pan);
				}
				break;
			}
			case ThreadCommandType::Pitch:
			{
				Playlist* playlist = nullptr;
				if (PlaylistManager::GetPlaylist(playlist, cmd.handle) == snd_err::OK)
				{
					playlist->Pitch(cmd.pitch);
				}
				break;
			}
			case ThreadCommandType::Volume:
			{
				Playlist* playlist = nullptr;
				if (PlaylistManager::GetPlaylist(playlist, cmd.handle) == snd_err::OK)
				{
					playlist->Volume(cmd.volume);
				}
				break;
			}
			default:
				break;
			}
			  
		}


		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));




	}

	SoundManager::Unload();

#ifdef _USE_TRACE_NEW
	_CrtDumpMemoryLeaks();
#endif



}