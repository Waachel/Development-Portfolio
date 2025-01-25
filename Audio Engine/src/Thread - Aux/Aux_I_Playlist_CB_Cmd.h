//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef AUX_I_PLAYLIST_CB_CMD_H
#define AUX_I_PLAYLIST_CB_CMD_H

#include "Command.h"
#include "I_Playlist_CB.h"

struct Aux_I_Playlist_CB_Cmd : public Command
{
	Aux_I_Playlist_CB_Cmd() = delete;
	Aux_I_Playlist_CB_Cmd(const Aux_I_Playlist_CB_Cmd &) = delete;
	Aux_I_Playlist_CB_Cmd &operator = (const Aux_I_Playlist_CB_Cmd &) = delete;
	~Aux_I_Playlist_CB_Cmd() = default;

	Aux_I_Playlist_CB_Cmd(I_Playlist_CB *pIFileCB);

	void Execute() override;


	I_Playlist_CB *pPlaylistCB;
};

#endif

//---  End of File ---

