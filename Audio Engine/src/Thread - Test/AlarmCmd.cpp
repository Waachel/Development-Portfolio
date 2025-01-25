//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AlarmCmd.h"
#include "SndMan.h"
#include "Snd.h"
#include "TimerMan.h"
#include "TimeEventMan.h"

using namespace ThreadFramework;


void AlarmCmd::Execute()
{

	Snd* pSndB = SndMan::Add(Snd::ID::Alarm);
	//pan left
	Handle::Status status1;
	status1 = pSndB->Pan(-1.0);
	status1 = pSndB->Vol(0.3f);
	status1 = pSndB->Play();

	//Snd::PrintPriorityTable();

}

// --- End of File ---
