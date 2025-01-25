//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "PanQuickChange.h"
#include "PanRight.h"
#include "PanLeft.h"
#include "BasicsDemo.h"
#include "SndMan.h"
#include "Snd.h"
#include <chrono>

using namespace ThreadFramework;
using namespace std::chrono_literals;

bool WaitForTimeQuickPan(Timer& t, Time TriggerTime)
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

void QuickPan_Main()
{
	Snd* pSndA = SndMan::Add(Snd::ID::SongA);

	PanLeft tmpLeft;
	tmpLeft.PanLeft_Main(pSndA);

	PanRight tmpRight;
	tmpRight.PanRight_Main();

	std::chrono::steady_clock::time_point TStartABegin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point TStartBBegin = std::chrono::steady_clock::now();



	Timer TE;
	TE.tic();


	if (WaitForTimeQuickPan(TE, 5 * Time(TIME_ONE_SECOND)))
	{
		std::chrono::steady_clock::time_point TStartACheck = std::chrono::steady_clock::now();
		Debug::out("104 Time Since Start: %d\n", std::chrono::duration_cast<std::chrono::seconds>(TStartACheck - TStartABegin).count());
	}

	if (WaitForTimeQuickPan(TE, 8 * Time(TIME_ONE_SECOND)))
	{
		std::chrono::steady_clock::time_point TStartACheck = std::chrono::steady_clock::now();
		Debug::out("104 Time Since Start: %d\n", std::chrono::duration_cast<std::chrono::seconds>(TStartACheck - TStartABegin).count());
	}

	if (WaitForTimeQuickPan(TE, 30 * Time(TIME_ONE_SECOND)))
	{
		std::chrono::steady_clock::time_point TStartACheck = std::chrono::steady_clock::now();
		Debug::out("104 Time Since Start: %d\n", std::chrono::duration_cast<std::chrono::seconds>(TStartACheck - TStartABegin).count());
		tmpLeft.PanLeft_Stop(pSndA);
	}

	if (WaitForTimeQuickPan(TE, 42 * Time(TIME_ONE_SECOND)))
	{
		std::chrono::steady_clock::time_point TStartBCheck = std::chrono::steady_clock::now();
		Debug::out("105 Time Since Start: %d\n", std::chrono::duration_cast<std::chrono::seconds>(TStartBCheck - TStartBBegin).count());
		tmpRight.PanRight_Stop();
	}
}


// End of File 