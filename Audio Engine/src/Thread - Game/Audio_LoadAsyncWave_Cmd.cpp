//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_LoadAsyncWave_Cmd.h"
#include "WaveMan.h"

using namespace ThreadFramework;

Audio_LoadAsyncWave_Cmd::Audio_LoadAsyncWave_Cmd(Wave::ID _id, const char* const _pWaveName, UserAsyncLoadCallback* _pUserAsyncLoadCallback)
	: Command(),
	id(_id),
	pWaveName(_pWaveName),
	pUserAsyncLoadCallback(_pUserAsyncLoadCallback)
{
	assert(pWaveName);
	assert(pUserAsyncLoadCallback);
}

void Audio_LoadAsyncWave_Cmd::Execute()
{
	Debug::out("Audio_LoadAsyncWave_Cmd::Execute(%s)\n", this->pWaveName);

	WaveMan::Add(this->id, this->pWaveName, this->pUserAsyncLoadCallback);

	delete this;
}


// --- End of File ---
