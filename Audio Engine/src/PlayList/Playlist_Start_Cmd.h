//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PLAYLIST_START_Cmd_H
#define PLAYLIST_START_Cmd_H

#include "PlaylistCmd.h"
#include "Voice.h"

class Playlist;

class Playlist_Start_Cmd : public PlaylistCmd
{

public:
	// Big 4v
	Playlist_Start_Cmd() = delete;
	Playlist_Start_Cmd(const Playlist_Start_Cmd &) = delete;
	Playlist_Start_Cmd &operator = (const Playlist_Start_Cmd &) = delete;
	~Playlist_Start_Cmd() = default;

	Playlist_Start_Cmd(Voice *pVoice, Playlist *pPlaylist);

	virtual void Execute() override;

public:
	// Data
	Voice *pVoice;
};

#endif

// --- End of File ---