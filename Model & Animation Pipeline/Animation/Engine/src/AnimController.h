//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectAnim.h"
#include "AnimWalk.h"
#include "AnimRun.h"
#include "AnimDie.h"
#include "AnimMain.h"
#include <AnimSide.h>

namespace Azul
{
	class AnimController
	{
	public:

		enum class Type
		{
			MAIN,
			WALK,
			RUN,
			DIE_LEFT,
			SIDE_STEP_RIGHT,
			NOT_INITIALIZED
		};

		AnimController(AnimTime delta, AnimController::Type type);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();
		TimerController* poTimerControl;

	private:
		
		AnimWalk *poAnimWalk;
		AnimRun* poAnimRun;
		AnimDie* poAnimDie;
		AnimSide* poAnimSide;
		AnimMain* poAnimMain;

		int animType = 0;
	};
}

#endif

// --- End of File ---
