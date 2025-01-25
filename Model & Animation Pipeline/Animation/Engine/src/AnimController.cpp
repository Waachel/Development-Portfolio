//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "Clip.h"
namespace Azul
{
	AnimController::AnimController(AnimTime delta, AnimController::Type type)
	{
		if (type == AnimController::Type::MAIN)
		{
			this->poAnimMain = new AnimMain();
			assert(this->poAnimMain);

			// TimerController
			AnimTime maxTime = poAnimMain->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 0;
		}


		// Animation object
		if (type == AnimController::Type::WALK)
		{
			this->poAnimWalk = new AnimWalk();
			assert(this->poAnimWalk);

			// TimerController
			AnimTime maxTime = poAnimWalk->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 1;
		}

		// Animation object
		if (type == AnimController::Type::DIE_LEFT)
		{
			this->poAnimDie = new AnimDie();
			assert(this->poAnimDie);

			// TimerController
			AnimTime maxTime = poAnimDie->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 2;
		}

		if (type == AnimController::Type::SIDE_STEP_RIGHT)
		{
			this->poAnimSide = new AnimSide();
			assert(this->poAnimSide);

			// TimerController
			AnimTime maxTime = poAnimSide->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 3;
		}

		if (type == AnimController::Type::RUN)
		{
			this->poAnimRun = new AnimRun();
			assert(this->poAnimRun);

			// TimerController
			AnimTime maxTime = poAnimRun->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 4;
		}


	}

	AnimController::~AnimController()
	{
		if (this->animType == 0)
		{
			delete poAnimMain;
		}
		if (this->animType == 1)
		{
			delete poAnimWalk;
		}
		if (this->animType == 2)
		{
			delete poAnimDie;
		}
		if (this->animType == 3)
		{
			delete poAnimSide;
		}
		if (this->animType == 4)
		{
			delete poAnimRun;
		}
		delete poTimerControl;
		this->animType = 0;
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		if (this->animType == 0)
		{
			this->poAnimMain->Animate(this->poTimerControl->GetCurrTime());
		}
		if (this->animType == 1)
		{
			this->poAnimWalk->Animate(this->poTimerControl->GetCurrTime());
		}
		if (this->animType == 2)
		{
			this->poAnimDie->Animate(this->poTimerControl->GetCurrTime());
		}
		if (this->animType == 3)
		{
			this->poAnimSide->Animate(this->poTimerControl->GetCurrTime());
		}
		if (this->animType == 4)
		{
			this->poAnimRun->Animate(this->poTimerControl->GetCurrTime());
		}
		
		
	}
}

// --- End of File ---
