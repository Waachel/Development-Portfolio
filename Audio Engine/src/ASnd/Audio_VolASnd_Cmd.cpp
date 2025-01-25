//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_VolASnd_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "ASndMan.h"

Audio_VolASnd_Cmd::Audio_VolASnd_Cmd(Snd::ID id, Snd *_pSnd, float _vol)
	: Command(),
	snd_id(id),
	pSnd(_pSnd),
	vol(_vol)
{
	assert(pSnd);
	assert(this->vol >= 0.0f);
}


void Audio_VolASnd_Cmd::Execute()
{

	assert(this->pSnd);

	ASnd *pASnd = pSnd->GetASnd();
	assert(pASnd);

	//Debug::out("Audio_VolASnd_Cmd::Execute() pSnd:%p pASnd:%p vol:%f\n",this->pSnd, pASnd, this->vol);
	pASnd->Vol(this->vol);

	delete this;
}


// --- End of File ---