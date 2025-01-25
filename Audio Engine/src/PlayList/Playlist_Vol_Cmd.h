//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Playlist_Vol_Cmd_H
#define Playlist_Vol_Cmd_H

#include "PlaylistCmd.h"
#include "Voice.h"

class Playlist;

class Playlist_Vol_Cmd : public PlaylistCmd
{

public:
	// Big 4v
	Playlist_Vol_Cmd() = delete;
	Playlist_Vol_Cmd(const Playlist_Vol_Cmd &) = delete;
	Playlist_Vol_Cmd &operator = (const Playlist_Vol_Cmd &) = delete;
	~Playlist_Vol_Cmd() = default;

	Playlist_Vol_Cmd(Voice *pVoice, Playlist *pPlaylist);

	virtual void Execute() override;

public:
	// Data
	Voice *pVoice;
};

#endif

// --- End of File ---