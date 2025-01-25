//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "PanRight.h"
#include "BasicsDemo.h"
#include "SndMan.h"

using namespace ThreadFramework;

Snd* pSnd;

void PanRight::PanRight_Main()
{
	SimpleBanner b;

	pSnd = SndMan::Add(Snd::ID::SongB);

	Debug::out("- Pan Right - \n");

	Handle::Status status1;
	status1 = pSnd->Vol(0.7f);
	status1 = pSnd->Pan(1.0);
	status1 = pSnd->Play();

}

void PanRight::PanRight_Stop()
{

	Debug::out("- Stop Right - \n");
	Handle::Status status1;
	status1 = pSnd->Stop();
}



// End of File 