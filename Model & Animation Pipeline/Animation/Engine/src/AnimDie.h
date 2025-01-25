//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_DIE_H
#define ANIM_DIE_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "Skeleton4.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimDie
	{
	public:
		AnimDie();
		AnimDie(const AnimDie&) = delete;
		AnimDie&operator = (const AnimDie&) = delete;
		~AnimDie();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		Skeleton4   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---