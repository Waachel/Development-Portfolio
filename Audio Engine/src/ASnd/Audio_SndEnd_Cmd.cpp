//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_SndEnd_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "ASndMan.h"

Audio_SndEnd_Cmd::Audio_SndEnd_Cmd(Snd::ID id, Snd *_pSnd)
	: Command(),
	snd_id(id),
	pSnd(_pSnd)
{
	assert(pSnd);
}


void Audio_SndEnd_Cmd::Execute()
{
	assert(this->pSnd);

	ASnd *pASnd = pSnd->GetASnd();
	assert(pASnd);

	//Debug::out("Audio_SndEnd_Cmd::Execute() pSnd:%p pASnd:%p\n",this->pSnd, pASnd);
	pASnd->SndEnd();

	delete this;
}


// --- End of File ---
