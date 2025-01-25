//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_TRANSFER_SND_USER_CALLBACK_COMMAND_H
#define GAME_TRANSFER_SND_USER_CALLBACK_COMMAND_H

#include "Command.h"
#include "UserSndCallback.h"

class Game_TransferSndUserCallback_Cmd : public Command
{
public:
	// Big 4
	Game_TransferSndUserCallback_Cmd() = delete;
	Game_TransferSndUserCallback_Cmd(const Game_TransferSndUserCallback_Cmd &) = delete;
	Game_TransferSndUserCallback_Cmd &operator = (const Game_TransferSndUserCallback_Cmd &) = delete;
	~Game_TransferSndUserCallback_Cmd() = default;

	Game_TransferSndUserCallback_Cmd(UserSndCallback *pUserCallback);

	virtual void Execute() override;

public:
	// Data
	UserSndCallback *pUserSndCallback;
};

#endif

// --- End of File ---
