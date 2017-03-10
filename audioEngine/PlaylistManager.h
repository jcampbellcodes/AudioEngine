#pragma once
#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include "SoundCallRegistry.h"
#include "snd_err.h"
#include <list>
class PlaylistFactory;
class Playlist;

// enums to playlist factories
struct PlaylistDescriptor
{
	PlaylistDescriptor() = default;
	~PlaylistDescriptor();
	PlaylistDescriptor(const PlaylistDescriptor&) = default;
	PlaylistDescriptor& operator=(const PlaylistDescriptor&) = default;


	SoundCallID playlistID;
	PlaylistFactory* factory = nullptr;
};

class PlaylistManager
{
public:

	static snd_err InitializePlaylistDescriptor(const char* path_to_script, SoundCallID id)
	{
		return Instance().privInitializePlaylistDescriptor(path_to_script, id);
	}

	static snd_err StartPlaylist(SoundCallID id, unsigned int handle)
	{
		return Instance().privStartPlaylist(id, handle);
	}

	static snd_err StopPlaylist(unsigned int handle)
	{
		return Instance().privStopPlaylist(handle);
	}

	static snd_err GetPlaylist(Playlist*& p, unsigned int handle)
	{
		return Instance().privGetPlaylist(p, handle);
	}

	static snd_err Terminate()
	{
		return Instance().privTerminate();
	}

private:
	// big four
	PlaylistManager();
	PlaylistManager(const PlaylistManager&) = delete;
	PlaylistManager& operator=(const PlaylistManager&) = delete;
	~PlaylistManager();
	// singleton 
	static PlaylistManager* instance;
	static PlaylistManager& Instance()
	{
		if (!instance)
		{
			instance = new PlaylistManager();
		}
		return *instance;
	}

	// members
	// a linked list of playlistID/factory pairs
	std::list<PlaylistDescriptor*> playlistDescriptors;

	// list of active playlists
	std::list<Playlist*> activePlaylists;

	// private methods
	snd_err privInitializePlaylistDescriptor(const char* path_to_script, SoundCallID id);
	snd_err privStartPlaylist(SoundCallID id, unsigned int handle);
	snd_err privStopPlaylist(unsigned int handle);

	snd_err privGetPlaylist(Playlist*& p, unsigned int handle);

	snd_err privTerminate();


};

#endif