//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game_TransferSndUserCallback_Cmd.h"
#include "WaveMan.h"
#include "StringThis.h"

using namespace ThreadFramework;

Game_TransferSndUserCallback_Cmd::Game_TransferSndUserCallback_Cmd(UserSndCallback *_pUserCallback)
	: Command(),
	pUserSndCallback(_pUserCallback)
{

}

void Game_TransferSndUserCallback_Cmd::Execute()
{
	//Debug::out("Game_TransferSndUserCallback_Cmd::Execute(%s)\n",
	//	StringMe(this->pUserSndCallback->GetSndID()) );

	if (this->pUserSndCallback != nullptr)
	{
		this->pUserSndCallback->Execute();
		delete this->pUserSndCallback;
	}

	delete this;
}


// --- End of File ---
