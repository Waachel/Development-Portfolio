//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_PanASnd_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "ASndMan.h"

Audio_PanASnd_Cmd::Audio_PanASnd_Cmd(Snd::ID id, Snd *_pSnd, float _vol)
	: Command(),
	snd_id(id),
	pSnd(_pSnd),
	pan(_vol)
{
	assert(pSnd);
	assert(this->pan >= -1.0f);
	assert(this->pan <= 1.0f);
}


void Audio_PanASnd_Cmd::Execute()
{

	assert(this->pSnd);

	ASnd *pASnd = pSnd->GetASnd();
	assert(pASnd);

	//Debug::out("Audio_PanASnd_Cmd::Execute() pSnd:%p pASnd:%p pan:%f\n",this->pSnd, pASnd, this->pan);
	pASnd->Pan(this->pan);

	delete this;
}


// --- End of File ---