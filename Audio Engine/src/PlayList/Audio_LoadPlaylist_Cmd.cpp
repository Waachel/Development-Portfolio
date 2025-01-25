//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_LoadPlaylist_Cmd.h"
#include "PlaylistJukeBox.h"
#include "Aux_I_Playlist_CB_Cmd.h"
#include "QueueMan.h"

using namespace ThreadFramework;

Audio_LoadPlaylist_Cmd::Audio_LoadPlaylist_Cmd(Snd::ID _snd_id, PlaylistData *pPlaylistData, I_Playlist_CB *_pPlaylistCB)
	: Command(),
	snd_id(_snd_id),
	poPlaylistData(pPlaylistData),
	pPlaylistCB(_pPlaylistCB)
{
	assert(pPlaylistCB);
	assert(poPlaylistData);
}


void Audio_LoadPlaylist_Cmd::Execute()
{
//	Debug::out("Audio_LoadPlaylist_Cmd::Execute()\n");

	Wave::ID wave_id = this->poPlaylistData->GetFirstWaveID();

	VoiceCallback *pCallback = this->poPlaylistData->Create_VoiceCallback();
	assert(pCallback);

	delete this->poPlaylistData;

	PlaylistJukeBox::Add(snd_id, wave_id, pCallback);

	Aux_I_Playlist_CB_Cmd *pCmd = new Aux_I_Playlist_CB_Cmd(this->pPlaylistCB);

	//	Debug::out("--> Aux_I_Playlist_CB_Cmd \n");
	bool s = QueueMan::SendAux(pCmd);
	assert(s);

	delete this;
}


// --- End of File ---