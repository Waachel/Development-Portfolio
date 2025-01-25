//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_Side_H
#define ANIM_Side_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "Skeleton3.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimSide
	{
	public:
		AnimSide();
		AnimSide(const AnimSide&) = delete;
		AnimSide&operator = (const AnimSide&) = delete;
		~AnimSide();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		Skeleton3   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---