//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef TIMER_MAN_H
#define TIMER_MAN_H

#include "AzulTimer.h"

class TimerMan
{
public:
	static void Create();
	static void Destroy();

	static void Update();
	static Time GetTimeCurrent();

private:
	TimerMan();
	TimerMan(const TimerMan &) = delete;
	TimerMan &operator = (const TimerMan &) = delete;
	~TimerMan();

	static TimerMan *privGetInstance();
	static TimerMan *posInstance;

	Timer mGameTimer;
	Time  mCurrTime;
	
};

#endif

//---  End of File ---
