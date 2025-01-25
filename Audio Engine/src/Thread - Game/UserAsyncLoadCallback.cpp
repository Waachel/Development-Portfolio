//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "UserAsyncLoadCallback.h"
#include "Snd.h"
#include "TimerMan.h"
#include "StringThis.h"
#include "TimeEventMan.h"
#include "PlaylistJukeBox.h"
#include "PlaylistData_One.h"
#include "SndMan.h"
#include "Audio.h"

using namespace ThreadFramework;

UserAsyncLoadCallback::UserAsyncLoadCallback()
	:pWaveName{ 0 },
	wave_id{ Wave::ID::Empty },
	timeStart(),
	timeEnd()
{

}

Wave::ID UserAsyncLoadCallback::GetWaveID()
{
	return this->wave_id;
}

void UserAsyncLoadCallback::Set(Wave::ID id, const char* _pWaveName)
{
	assert(_pWaveName);
	memset(this->pWaveName, 0, Wave::NAME_SIZE);
	strcpy_s(this->pWaveName, Wave::NAME_SIZE, _pWaveName);
	this->wave_id = id;

	this->timeStart = TimeEventMan::GetTimeCurrent();
	this->timeEnd = TimeEventMan::GetTimeCurrent();
}

void UserAsyncLoadCallback::Execute()
{
	assert(this->pWaveName);

	Debug::out("------------------------------\n");
	Debug::out("  UserAsyncLoadCallback(%p) \n", this);
	Debug::out("\n");
	Debug::out("    Wave:ID: %d (%s)\n", this->wave_id, StringMe(this->wave_id));
	Debug::out("       Wave: %s \n", this->pWaveName);


	this->timeEnd = TimeEventMan::GetTimeCurrent();
	Time delta = this->timeEnd - this->timeStart;
	int ms = Time::quotient(delta, Time(Duration::TIME_ONE_MILLISECOND));
	Debug::out("   duration: %f s (%d ms) \n", (float)ms / 1000.0f, ms);

	Debug::out("------------------------------\n");


	// As soon as the Beethoven is loaded… from the callback
	//	  Start the Beethoven sound
	//	  SndC = Play 503, vol: 50 %, pan : center, stereo, Priority default (optional)
	//	     Beethoven should start
	//       Debug::out() in the callback
	//	     Print the wave table

	PlaylistJukeBox::Load(Snd::ID::Beethoven, new PlaylistData_One(Wave::ID::Beethoven));

	// ---------------------------------------
	// Play C
	// ---------------------------------------

	Snd* pSndC = SndMan::Add(Snd::ID::Beethoven);
	assert(pSndC);

	// Vol & Pan
	assert(pSndC->Vol(15.0f) == Handle::Status::SUCCESS);
	assert(pSndC->Pan(0.0f) == Handle::Status::SUCCESS);

	// Call the sound
	assert(pSndC->Play() == Handle::Status::SUCCESS);

	// ---------------------------------------
	// Priority Table
	// ---------------------------------------
	Snd::PrintPriorityTable();
	Audio::WaveTableDump();

}


// --- End of File ---
