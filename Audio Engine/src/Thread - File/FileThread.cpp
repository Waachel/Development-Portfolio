//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

using namespace ThreadFramework;

#include "QueueMan.h"
#include "Command.h"

void FileMain(std::atomic_bool &QuitFlag)
{
	SimpleBanner b;

	CircularData *pFileIn = QueueMan::GetFileInQueue();

	// ----------------------------------------
	// Loop for ever until quit is hit
	// ----------------------------------------
	while(!QuitFlag)
	{
		Command *pCmd;

		if(pFileIn->PopFront(pCmd) == true)
		{
			assert(pCmd);
			pCmd->Execute();
		}
	}

}

//---  End of File ---