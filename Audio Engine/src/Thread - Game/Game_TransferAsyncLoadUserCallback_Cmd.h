//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_TRANSFER_ASYNC_LOAD_USER_CALLBACK_COMMAND_H
#define GAME_TRANSFER_ASYNC_LOAD_USER_CALLBACK_COMMAND_H

#include "Command.h"
#include "UserAsyncLoadCallback.h"

class Game_TransferAsyncLoadUserCallback_Cmd : public Command
{
public:
	// Big 4
	Game_TransferAsyncLoadUserCallback_Cmd() = delete;
	Game_TransferAsyncLoadUserCallback_Cmd(const Game_TransferAsyncLoadUserCallback_Cmd&) = delete;
	Game_TransferAsyncLoadUserCallback_Cmd& operator = (const Game_TransferAsyncLoadUserCallback_Cmd&) = delete;
	~Game_TransferAsyncLoadUserCallback_Cmd() = default;

	Game_TransferAsyncLoadUserCallback_Cmd(UserAsyncLoadCallback* pUserAsyncLoadCallback);

	virtual void Execute() override;

public:
	// Data
	UserAsyncLoadCallback* pUserAsyncLoadCallback;
};

#endif

// --- End of File ---
