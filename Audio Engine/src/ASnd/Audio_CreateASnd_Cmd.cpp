//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Audio_CreateASnd_Cmd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "ASndMan.h"

using namespace ThreadFramework;

Audio_CreateASnd_Cmd::Audio_CreateASnd_Cmd(Snd::ID id, Snd *_pSnd, UserSndCallback *_pUserSndCallback)
	: Command(),
	snd_id(id),
	pSnd(_pSnd),
	pUserSndCallback(_pUserSndCallback)
{
	assert(pSnd);
}


void Audio_CreateASnd_Cmd::Execute()
{
//	Debug::out("Audio_CreateASnd_Cmd::Execute()\n");

	assert(this->pSnd);

	ASnd *pASnd = ASndMan::Add(this->snd_id, this->pSnd);
	assert(pASnd);

	// user callback
	pASnd->pUserSndCallback = this->pUserSndCallback;

	//Debug::out("Audio_CreateASnd_Cmd():Snd:%p ASnd:%p\n",pSnd, pASnd);
	delete this;
}


// --- End of File ---