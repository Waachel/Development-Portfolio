//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Playlist_COMMAND_H
#define Playlist_COMMAND_H

#include "Handle.h"

class Playlist;

class PlaylistCmd
{
public:
	enum Type
	{
		Play,
		Stop,
		Uninitialized
	};

public:
	// Big 4
	PlaylistCmd();
	PlaylistCmd(const PlaylistCmd &) = default;
	PlaylistCmd &operator = (const PlaylistCmd &) = default;
	virtual ~PlaylistCmd() = default;

	PlaylistCmd(Playlist *pPlaylist);

	virtual void Execute() = 0;

public:
	// Data
	Playlist *pPlaylist;

};

#endif

// --- End of File ---