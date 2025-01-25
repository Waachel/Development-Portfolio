//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

using namespace ThreadFramework;

#include "Aux_I_Playlist_CB_Cmd.h"

Aux_I_Playlist_CB_Cmd::Aux_I_Playlist_CB_Cmd(I_Playlist_CB *_pIFileCB)
	: pPlaylistCB(_pIFileCB)
{
	assert(pPlaylistCB);
}

void Aux_I_Playlist_CB_Cmd::Execute()
{
//	Debug::out("Aux_I_Playlist_CB_Cmd::Execute()\n");

	this->pPlaylistCB->Execute();

	delete this;
}

// --- End of File ---
