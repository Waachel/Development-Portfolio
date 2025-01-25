//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_RUN_H
#define ANIM_RUN_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "Skeleton2.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimRun
	{
	public:
		AnimRun();
		AnimRun(const AnimRun&) = delete;
		AnimRun&operator = (const AnimRun&) = delete;
		~AnimRun();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		Skeleton2   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---