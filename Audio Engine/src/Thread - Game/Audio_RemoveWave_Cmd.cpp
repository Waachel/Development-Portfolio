//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_RemoveWave_Cmd.h"
#include "WaveMan.h"
#include "StringThis.h"

using namespace ThreadFramework;

Audio_RemoveWave_Cmd::Audio_RemoveWave_Cmd(Wave::ID _id, I_File_CB* _pFileCB)
	: Command(),
	id(_id),
	pIFileCB(_pFileCB)
{
	assert(pIFileCB);
}

void Audio_RemoveWave_Cmd::Execute()
{
	Debug::out("Audio_RemoveWave_Cmd::Execute(%s)\n", StringMe(this->id));

	WaveMan::Remove(this->id, this->pIFileCB);

	delete this;
}


// --- End of File ---
