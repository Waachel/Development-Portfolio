//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_RemoveAllWaves_Cmd.h"
#include "WaveMan.h"
#include "StringThis.h"

using namespace ThreadFramework;

Audio_RemoveAllWaves_Cmd::Audio_RemoveAllWaves_Cmd(I_File_CB* _pFileCB)
	: Command(),
	pIFileCB(_pFileCB)
{
	assert(pIFileCB);
}

void Audio_RemoveAllWaves_Cmd::Execute()
{
	Debug::out("Audio_RemoveAllWaves_Cmd::Execute()\n");

	WaveMan::RemoveAll(this->pIFileCB);

	delete this;
}


// --- End of File ---
