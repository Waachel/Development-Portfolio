//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Playlist_Vol_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "SndMan.h"
#include "PlayList.h"

using namespace ThreadFramework;

Playlist_Vol_Cmd::Playlist_Vol_Cmd(Voice *_pVoice, Playlist *pPlaylist)
	: PlaylistCmd(pPlaylist),
	pVoice(_pVoice)
{
	assert(pPlaylist);
	assert(pVoice);
}


void Playlist_Vol_Cmd::Execute()
{
//	Debug::out("Playlist_Vol_Cmd::Execute()\n");

	// -------------------------------------------------------
	// Start the source voice
	// -------------------------------------------------------
	assert(pVoice);
	Handle::Status status;
	status = pVoice->Vol(this->pPlaylist->vol);
	assert(status == Handle::Status::SUCCESS);

//	delete this;
}


// --- End of File ---