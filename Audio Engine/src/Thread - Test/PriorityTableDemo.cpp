//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PriorityTableDemo.h"
#include "AzulCore.h"
#include "SndMan.h"
#include "AlarmCmd.h"
#include "TimeEventMan.h"
#include "UserAsyncLoadCallback.h"
#include "Audio.h"
#include "PlaylistData_One.h"
#include "PlaylistJukeBox.h"

using namespace ThreadFramework;
using namespace std::chrono_literals;

bool PTWaitForTime(Timer& t, Time TriggerTime)
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

void LoadDemo3()
{
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Coma, "Coma_mono.wav");

	// Create the Playlist
	PlaylistJukeBox::Load(Snd::ID::Coma, new PlaylistData_One(Wave::ID::Coma));
}

void KillDemo3()
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

void PriorityTableDemoMain()
{
	SimpleBanner b;

	LoadDemo3();

	Timer T1;
	T1.tic();

	Snd* pSndA = SndMan::Add(Snd::ID::Coma, 10);
	Handle::Status status1;
	//pan center
	status1 = pSndA->Pan(0.0);
	status1 = pSndA->Vol(0.1f);
	status1 = pSndA->Play();

	Snd* pSndB = SndMan::Add(Snd::ID::Coma, 50);
	status1 = pSndB->Pan(0.0);
	status1 = pSndB->Vol(0.1f);
	status1 = pSndB->Play();

	Snd* pSndC = SndMan::Add(Snd::ID::Coma, 150);
	status1 = pSndC->Pan(0.0);
	status1 = pSndC->Vol(0.1f);
	status1 = pSndC->Play();

	Snd::PrintPriorityTable();

	if (PTWaitForTime(T1, 1 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndD = SndMan::Add(Snd::ID::Coma, 50);
		status1 = pSndD->Pan(0.0);
		status1 = pSndD->Vol(0.1f);
		status1 = pSndD->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 2 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndE = SndMan::Add(Snd::ID::Coma, 75);
		status1 = pSndE->Pan(0.0);
		status1 = pSndE->Vol(0.1f);
		status1 = pSndE->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 3 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndF = SndMan::Add(Snd::ID::Coma, 100);

		status1 = pSndF->Pan(0.0);
		status1 = pSndF->Vol(0.1f);
		status1 = pSndF->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 4 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndG = SndMan::Add(Snd::ID::Coma, 150);

		status1 = pSndG->Pan(0.0);
		status1 = pSndG->Vol(0.1f);
		status1 = pSndG->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 5 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndH = SndMan::Add(Snd::ID::Coma, 75);

		status1 = pSndH->Pan(0.0);
		status1 = pSndH->Vol(0.1f);
		status1 = pSndH->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 6 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndI = SndMan::Add(Snd::ID::Coma, 75);

		status1 = pSndI->Pan(0.0);
		status1 = pSndI->Vol(0.1f);
		status1 = pSndI->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 7 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndJ = SndMan::Add(Snd::ID::Coma, 75);

		status1 = pSndJ->Pan(0.0);
		status1 = pSndJ->Vol(0.1f);
		status1 = pSndJ->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 8 * Time(TIME_ONE_SECOND)))
	{
		Snd* pSndK = SndMan::Add(Snd::ID::Coma, 150);

		status1 = pSndK->Pan(0.0);
		status1 = pSndK->Vol(0.1f);
		status1 = pSndK->Play();

		Snd::PrintPriorityTable();
	}

	if (PTWaitForTime(T1, 13 * Time(TIME_ONE_SECOND)))
	{

		Snd::PrintPriorityTable();

		KillDemo3();

	}
}


//---  End of File ---
