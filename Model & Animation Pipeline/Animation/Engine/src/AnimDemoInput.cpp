//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimDemoInput.h"
#include "AnimMain.h"
#include "TimerController.h"

namespace Azul
{
	void AnimDemoInput(Clip * c, AnimController * a)
	{
		//Play
		if ((GetKeyState('G') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Pause
		if ((GetKeyState('P') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.0f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Forward
		if ((GetKeyState('F') & 0x8000))
		{
			a->poTimerControl->SetForRev(0);
		}
		//Backward
		if ((GetKeyState('B') & 0x8000))
		{
			a->poTimerControl->SetForRev(1);
		}

		//Slow
		if ((GetKeyState('L') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.1f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Normal
		if ((GetKeyState('N') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Fast
		if ((GetKeyState('T') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.4f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Looping
		
		//Switching Animations
		if((GetKeyState('W') & 0x8000))
		{
			c->ChangeAnim(1);
		}

		if ((GetKeyState('D') & 0x8000))
		{
			c->ChangeAnim(2);
		}

		if ((GetKeyState('S') & 0x8000))
		{
			c->ChangeAnim(3);
		}

		if ((GetKeyState('R') & 0x8000))
		{
			c->ChangeAnim(4);
		}


	}

}

// --- End of File ---
