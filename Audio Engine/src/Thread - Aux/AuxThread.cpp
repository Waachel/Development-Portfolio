//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

using namespace ThreadFramework;

#include "QueueMan.h"
#include "Command.h"

void AuxMain(std::atomic_bool &QuitFlag)
{
	SimpleBanner b;

	CircularData *pAuxIn = QueueMan::GetAuxInQueue();

	// ----------------------------------------
	// Loop for ever until quit is hit
	// ----------------------------------------
	while(!QuitFlag)
	{
		Command *pCmd;

		if(pAuxIn->PopFront(pCmd) == true)
		{
			assert(pCmd);
			pCmd->Execute();
		}
	}
}

//---  End of File ---
