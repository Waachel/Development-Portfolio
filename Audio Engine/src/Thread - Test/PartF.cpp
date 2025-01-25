//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "PartF.h"
#include "BasicsDemo.h"
#include "SndMan.h"

using namespace ThreadFramework;

// cool way to deal with time...
using namespace std::chrono_literals;


void PartF_Main()
{
	Snd* pSndA = SndMan::Add(Snd::ID::Bassoon);
	Handle::Status status1 = pSndA->Vol(0.4f);
	status1 = pSndA->Play();

	std::this_thread::sleep_for(500ms);

	Debug::out("Sound 1\n");

	Snd* pSndB = SndMan::Add(Snd::ID::Bassoon);
	status1 = pSndB->Vol(0.4f);
	status1 = pSndB->Play();

	Debug::out("Sound 2\n");

	std::this_thread::sleep_for(500ms);

	Snd* pSndC = SndMan::Add(Snd::ID::Bassoon);
	status1 = pSndC->Vol(0.4f);
	status1 = pSndC->Play();

	Debug::out("Sound 3\n");

	std::this_thread::sleep_for(500ms);

	Snd* pSndD = SndMan::Add(Snd::ID::Bassoon);
	status1 = pSndD->Vol(0.4f);
	status1 = pSndD->Play();

	Debug::out("Sound 4\n");

	status1 = pSndA->Stop();
	status1 = pSndB->Stop();
	status1 = pSndC->Stop();
}



// End of File 