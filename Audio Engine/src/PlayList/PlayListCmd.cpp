//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PlayListCmd.h"
#include "Playlist.h"

PlaylistCmd:: PlaylistCmd()
	:pPlaylist(nullptr)
{

}

PlaylistCmd::PlaylistCmd(Playlist *_pPlaylist)
	: pPlaylist(_pPlaylist)
{

}

// --- End of File ---
