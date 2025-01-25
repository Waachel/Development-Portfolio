//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "BasicsDemo.h"
#include "AzulCore.h"
#include "SndMan.h"
#include "TimeEventMan.h"
#include "UserAsyncLoadCallback.h"
#include "Audio.h"
#include "PlaylistData_One.h"
#include "PlaylistJukeBox.h"
#include "LeftRightPan.h"
#include "PanQuickChange.h"
#include "PartF.h"
#include "VolumeChange.h"

using namespace ThreadFramework;
using namespace std::chrono_literals;

bool BasicsWaitForTime(Timer& t, Time TriggerTime)
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

void LoadDemo1()
{
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Fiddle, "Fiddle_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Bassoon, "Bassoon_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Oboe2_mono, "Oboe2_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::SongA, "SongA.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::SongB, "SongB.wav");


	// Create the Playlist
	PlaylistJukeBox::Load(Snd::ID::Fiddle, new PlaylistData_One(Wave::ID::Fiddle));
	PlaylistJukeBox::Load(Snd::ID::Bassoon, new PlaylistData_One(Wave::ID::Bassoon));
	PlaylistJukeBox::Load(Snd::ID::Oboe2_mono, new PlaylistData_One(Wave::ID::Oboe2_mono));
	PlaylistJukeBox::Load(Snd::ID::SongA, new PlaylistData_One(Wave::ID::SongA));
	PlaylistJukeBox::Load(Snd::ID::SongB, new PlaylistData_One(Wave::ID::SongB));
}

void KillDemo1()
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

void BasicsDemoMain()
{
	SimpleBanner b;
	LoadDemo1();

	Timer T1;
	T1.tic();

	Snd* pSndA = SndMan::Add(Snd::ID::Fiddle);

	Handle::Status status1;
	status1 = pSndA->Vol(0.7f);
	status1 = pSndA->Play();

	if (BasicsWaitForTime(T1, 3 * Time(TIME_ONE_SECOND)))
	{
		pSndA = SndMan::Add(Snd::ID::Fiddle);
		status1 = pSndA->Vol(0.7f);
		status1 = pSndA->Pan(-1.0);
		status1 = pSndA->Play();

	}

	if (BasicsWaitForTime(T1, 6 * Time(TIME_ONE_SECOND)))
	{
		//status = pSndA->Pan(1.0);
		pSndA = SndMan::Add(Snd::ID::Fiddle);
		status1 = pSndA->Vol(0.7f);
		status1 = pSndA->Pan(1.0);
		status1 = pSndA->Play();
;
	}

	if (BasicsWaitForTime(T1, 10 * Time(TIME_ONE_SECOND)))
	{
		std::thread t(LeftRightPan_Main);
		Debug::SetName(t, "-LeftRightPan-");
		t.join();
	}

	if (BasicsWaitForTime(T1, 15 * Time(TIME_ONE_SECOND)))
	{
		std::thread t(RightLeftPan_Main);
		Debug::SetName(t, "-RightLeftPan-");
		t.join();
	}

	if (BasicsWaitForTime(T1, 20 * Time(TIME_ONE_SECOND)))
	{
		std::thread t(VolumeUp_Main);
		Debug::SetName(t, "-Volume Up-");
		t.join();
	}

	if (BasicsWaitForTime(T1, 25 * Time(TIME_ONE_SECOND)))
	{
		std::thread t(VolumeDown_Main);
		Debug::SetName(t, "-Volume Down-");
		t.join();
	}

	if (BasicsWaitForTime(T1, 30 * Time(TIME_ONE_SECOND)))
	{
		std::thread t1(QuickPan_Main);
		Debug::SetName(t1, "-Part E-");
		t1.join();
	}

	if (BasicsWaitForTime(T1, 80 * Time(TIME_ONE_SECOND)))
	{
		std::thread t1(PartF_Main);
		Debug::SetName(t1, "-Part F-");
		t1.join();
		//KillDemo1();
	}

	if (BasicsWaitForTime(T1, 90 * Time(TIME_ONE_SECOND)))
	{
		KillDemo1();
	}


}


//---  End of File ---
