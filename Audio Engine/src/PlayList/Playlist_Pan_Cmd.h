//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PLAYLIST_PAN_Cmd_H
#define PLAYLIST_PAN_Cmd_H

#include "PlaylistCmd.h"
#include "Voice.h"

class Playlist;

class Playlist_Pan_Cmd : public PlaylistCmd
{

public:
	// Big 4v
	Playlist_Pan_Cmd() = delete;
	Playlist_Pan_Cmd(const Playlist_Pan_Cmd&) = delete;
	Playlist_Pan_Cmd& operator = (const Playlist_Pan_Cmd&) = delete;
	~Playlist_Pan_Cmd() = default;

	Playlist_Pan_Cmd(Voice *pVoice, Playlist *pPlaylist);

	virtual void Execute() override;

public:
	// Data
	Voice* pVoice;
};

#endif

// --- End of File ---