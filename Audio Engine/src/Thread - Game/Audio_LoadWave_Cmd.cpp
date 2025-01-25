//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_LoadWave_Cmd.h"
#include "WaveMan.h"

using namespace ThreadFramework;

Audio_LoadWave_Cmd::Audio_LoadWave_Cmd(Wave::ID _id, const char *const _pWaveName, I_File_CB *_pFileCB)
	: Command(),
	id(_id),
	pWaveName(_pWaveName),
	pIFileCB(_pFileCB)
{
	assert(pWaveName);
	assert(pIFileCB);
}

void Audio_LoadWave_Cmd::Execute()
{
	Debug::out("Audio_LoadWave_Cmd::Execute(%s)\n", this->pWaveName);

	WaveMan::Add(this->id, this->pWaveName, this->pIFileCB);

	delete this;
}


// --- End of File ---