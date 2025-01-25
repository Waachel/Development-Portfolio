//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Playlist_Start_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "SndMan.h"
#include "PlayList.h"

using namespace ThreadFramework;

Playlist_Start_Cmd::Playlist_Start_Cmd(Voice *_pVoice, Playlist *pPlaylist)
	: PlaylistCmd(pPlaylist),
	pVoice(_pVoice)
{
	assert(pPlaylist);
	assert(pVoice);
}


void Playlist_Start_Cmd::Execute()
{
//	Debug::out("Playlist_Start_Cmd::Execute()\n");

	// -------------------------------------------------------
	// Start the source voice
	// -------------------------------------------------------
	assert(pVoice);
	Handle::Status status;
	status = pVoice->Start();
	assert(status == Handle::Status::SUCCESS);

//	delete this;
}


// --- End of File ---