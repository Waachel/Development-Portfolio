//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_PlayASnd_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "ASndMan.h"
#include "StringThis.h"

Audio_PlayASnd_Cmd::Audio_PlayASnd_Cmd(Snd::ID id, Snd *_pSnd)
	: Command(),
	snd_id(id),
	pSnd(_pSnd)
{
	assert(pSnd);
}


void Audio_PlayASnd_Cmd::Execute()
{

	assert(this->pSnd);

	ASnd *pASnd = pSnd->GetASnd();
	assert(pASnd);

	//Debug::out("Audio_PlayASnd_Cmd::Execute(%s) \n", StringMe(this->pSnd->snd_id));
	pASnd->Play();

	delete this;
}


// --- End of File ---