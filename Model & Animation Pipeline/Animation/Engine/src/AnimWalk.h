//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_WALK_H
#define ANIM_WALK_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "Skeleton1.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimWalk
	{
	public:
		AnimWalk();
		AnimWalk(const AnimWalk&) = delete;
		AnimWalk&operator = (const AnimWalk&) = delete;
		~AnimWalk();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		Skeleton1   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---