#include "StartPlaylistCommand.h"
#include "PlaylistManager.h"

StartPlaylistCommand::StartPlaylistCommand() : id(), handle(0)
{
}

StartPlaylistCommand::StartPlaylistCommand(SoundCallID _id, unsigned int _handle) : id(_id), handle(_handle)
{
}



snd_err StartPlaylistCommand::SetContext(SoundCallID _id, unsigned int _handle)
{
	snd_err err = snd_err::OK;

	id = _id;
	handle = _handle;

	return err;
}

void StartPlaylistCommand::execute()
{
	PlaylistManager::StartPlaylist(id, handle);
}
