//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MAIN_H
#define ANIM_MAIN_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "SkeletonMain.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimMain
	{
	public:
		AnimMain();
		AnimMain(const AnimMain&) = delete;
		AnimMain&operator = (const AnimMain&) = delete;
		~AnimMain();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		void ChangeAnimType(int i);

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		SkeletonMain   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---