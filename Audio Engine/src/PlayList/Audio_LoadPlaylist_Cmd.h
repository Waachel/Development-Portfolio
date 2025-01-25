//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_PLAYLIST_LOAD_COMMAND_H
#define AUDIO_PLAYLIST_LOAD_COMMAND_H

#include "Command.h"
#include "Snd.h"
#include "Wave.h"
#include "Playlist.h"
#include "PlaylistData.h"
#include "I_Playlist_CB.h"


class Audio_LoadPlaylist_Cmd : public Command
{

public:
	// Big 4
	 Audio_LoadPlaylist_Cmd() = delete;
	 Audio_LoadPlaylist_Cmd(const Audio_LoadPlaylist_Cmd &) = delete;
	 Audio_LoadPlaylist_Cmd &operator = (const Audio_LoadPlaylist_Cmd &) = delete;
	~Audio_LoadPlaylist_Cmd() = default;

	Audio_LoadPlaylist_Cmd(Snd::ID snd_id, PlaylistData *pPlaylistData, I_Playlist_CB *pIPlaylistCB);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	PlaylistData *poPlaylistData;
	I_Playlist_CB *pPlaylistCB;
};

#endif

// --- End of File ---