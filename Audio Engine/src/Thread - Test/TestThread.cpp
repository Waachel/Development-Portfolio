//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "TestThread.h"
#include "AzulCore.h"
#include "LeftRightPan.h"

using namespace ThreadFramework;

bool WaitForTime(Timer &t, Time TriggerTime)
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

void TestMain()
{
	SimpleBanner b;

	Timer T1;
	T1.tic();

	if (WaitForTime(T1, Time(TIME_ZERO)))
	{
		Debug::out("-  0 Sec - Part A: center\n");
	}

	if (WaitForTime(T1, 3 * Time(TIME_ONE_SECOND)))
	{
		Debug::out("-  3 Sec - Part A: left\n");
	}

	if (WaitForTime(T1, 6 * Time(TIME_ONE_SECOND)))
	{
		Debug::out("-  6 Sec - Part A: right\n");
	}

	if (WaitForTime(T1, 10 * Time(TIME_ONE_SECOND)))
	{
		Debug::out("- 10 Sec - Part B: PanLeftToRight\n");

		std::thread t(LeftRightPan_Main);
		Debug::SetName(t, "-LeftRightPan-");
		t.join();
	}

	if (WaitForTime(T1, 15 * Time(TIME_ONE_SECOND)))
	{
		Debug::out("- 15 Sec - Part B: blah blah\n");
	}

}

//---  End of File ---
