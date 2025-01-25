//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PLAYLIST_STOP_Cmd_H
#define PLAYLIST_STOP_Cmd_H

#include "PlaylistCmd.h"
#include "Voice.h"

class Playlist;

class Playlist_Stop_Cmd : public PlaylistCmd
{

public:
	// Big 4v
	Playlist_Stop_Cmd() = delete;
	Playlist_Stop_Cmd(const Playlist_Stop_Cmd &) = delete;
	Playlist_Stop_Cmd &operator = (const Playlist_Stop_Cmd &) = delete;
	~Playlist_Stop_Cmd() = default;

	Playlist_Stop_Cmd(Voice *pVoice, Playlist *pPlaylist);

	virtual void Execute() override;

public:
	// Data
	Voice *pVoice;
};

#endif

// --- End of File ---