//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "UserCallbacksDemo.h"
#include "AzulCore.h"
#include "SndMan.h"
#include "AlarmCmd.h"
#include "TimeEventMan.h"
#include "UserAsyncLoadCallback.h"
#include "Audio.h"
#include "PlaylistData_One.h"
#include "PlaylistJukeBox.h"
#include "UserSndCallback.h"

using namespace ThreadFramework;
using namespace std::chrono_literals;

bool UCWaitForTime(Timer& t, Time TriggerTime)
{
	Time deltaTime(TIME_ZERO);

	if (deltaTime != TriggerTime)
	{
		while (!Time::quotient(deltaTime, TriggerTime))
		{
			// Spin
			deltaTime = t.toc();
		}
	}
	return true;
}

void LoadDemo4()
{
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Dial, "Dial_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::MoonPatrol, "MoonPatrol_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Sequence, "Sequence1_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Donkey, "Donkey_mono.wav");
	// Create the Playlist
	PlaylistJukeBox::Load(Snd::ID::Dial, new PlaylistData_One(Wave::ID::Dial));
	PlaylistJukeBox::Load(Snd::ID::MoonPatrol, new PlaylistData_One(Wave::ID::MoonPatrol));
	PlaylistJukeBox::Load(Snd::ID::Sequence, new PlaylistData_One(Wave::ID::Sequence));
	PlaylistJukeBox::Load(Snd::ID::Donkey, new PlaylistData_One(Wave::ID::Donkey));
}

void KillDemo4()
{
	// ---------------------------------------
	// Priority Table
	// ---------------------------------------
	Snd::KillAllActive();
	Snd::PrintPriorityTable();

	// ---------------------------------------
	// Wave Table
	// ---------------------------------------
	Audio::WaveTableDump();
	Audio::RemoveAllWaves();
	Audio::WaveTableDump();
}

void UserCallbacksDemoMain()
{
	SimpleBanner b;

	LoadDemo4();

	Timer T1;
	T1.tic();

	UserSndCallback* pUserSndCallbackA = new UserSndCallback();
	Snd* pSndA = SndMan::Add(Snd::ID::Dial, pUserSndCallbackA);

	Handle::Status status1;
	status1 = pSndA->Pan(-1.0);
	status1 = pSndA->Vol(0.6f);
	status1 = pSndA->Play();

	UserSndCallback* pUserSndCallbackB = new UserSndCallback();
	Snd* pSndB = SndMan::Add(Snd::ID::MoonPatrol, pUserSndCallbackB);

	status1 = pSndB->Pan(1.0);
	status1 = pSndB->Vol(0.6f);
	status1 = pSndB->Play();

	UserSndCallback* pUserSndCallbackC = new UserSndCallback();
	Snd* pSndC = SndMan::Add(Snd::ID::Sequence, pUserSndCallbackC);

	status1 = pSndC->Pan(-1.0);
	status1 = pSndC->Vol(0.6f);
	status1 = pSndC->Play();

	if (UCWaitForTime(T1, 3500 * Time(TIME_ONE_MILLISECOND)))
	{
		UserSndCallback* pUserSndCallbackD = new UserSndCallback();
		Snd* pSndD = SndMan::Add(Snd::ID::Donkey, pUserSndCallbackD);

		status1 = pSndD->Pan(1.0);
		status1 = pSndD->Vol(0.6f);
		status1 = pSndD->Play();

	}

	if (UCWaitForTime(T1, 28 * Time(TIME_ONE_SECOND)))
	{
		KillDemo4();
	}

	
	

}


//---  End of File ---
