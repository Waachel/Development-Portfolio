//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "UserSndCallback.h"
#include "Snd.h"
#include "TimerMan.h"
#include "StringThis.h"

using namespace ThreadFramework;

void UserSndCallback::Set(ASnd *_pASnd, Snd *_pSnd, char *_pWaveName)
{
	assert(_pSnd);
	this->pSnd = _pSnd;
	assert(_pWaveName);
	memset(this->pWaveName, 0, Wave::NAME_SIZE);
	strcpy_s(this->pWaveName, Wave::NAME_SIZE, _pWaveName);
	this->snd_id = pSnd->snd_id;

	this->vol = _pASnd->pPlaylist->vol;
	this->pan = _pASnd->pPlaylist->pan;
	this->timeStart = pSnd->GetTime();
	this->timeEnd = TimerMan::GetTimeCurrent();
}

void UserSndCallback::Execute()
{
	assert(this->pSnd);
	assert(this->pWaveName);

	Debug::out("------------------------------\n");
	Debug::out("  UserSndCallback(%p) \n", this);
	Debug::out("\n");
	Debug::out("       pSnd: %p \n", this->pSnd);
	Debug::out("     Snd:ID: %d (%s)\n", this->snd_id,StringMe(this->snd_id));
	Debug::out("       Wave: %s \n", this->pWaveName);
	Debug::out("        Vol: %f \n", this->vol);
	Debug::out("        Pan: %f \n", this->pan);

	Time delta = this->timeEnd - this->timeStart;
	int ms = Time::quotient(delta, Time(Duration::TIME_ONE_MILLISECOND));
	Debug::out("   duration: %f s (%d ms) \n", (float)ms/1000.0f, ms);

	Debug::out("------------------------------\n");

	Snd::PrintPriorityTable();
}

Snd::ID UserSndCallback::GetSndID()
{
	return this->snd_id;
}

// --- End of File ---
