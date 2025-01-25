//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game_TransferAsyncLoadUserCallback_Cmd.h"
#include "WaveMan.h"
#include "StringThis.h"

using namespace ThreadFramework;

Game_TransferAsyncLoadUserCallback_Cmd::Game_TransferAsyncLoadUserCallback_Cmd(UserAsyncLoadCallback* _pUserAsyncLoadCallback)
	: Command(),
	pUserAsyncLoadCallback(_pUserAsyncLoadCallback)
{

}

void Game_TransferAsyncLoadUserCallback_Cmd::Execute()
{
	Debug::out("Game_TransferAsyncLoadUserCallback_Cmd::Execute(%s)\n",
		StringMe(this->pUserAsyncLoadCallback->GetWaveID()));

	if (this->pUserAsyncLoadCallback != nullptr)
	{
		this->pUserAsyncLoadCallback->Execute();
		delete this->pUserAsyncLoadCallback;
	}

	delete this;
}


// --- End of File ---
