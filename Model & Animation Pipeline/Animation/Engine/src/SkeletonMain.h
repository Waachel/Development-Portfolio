//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SkeletonMain_H
#define SkeletonMain_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class SkeletonMain
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		SkeletonMain(Bone *pBone, int numBones);

		SkeletonMain() = delete;
		SkeletonMain(const SkeletonMain &) = delete;
		SkeletonMain &operator = (const SkeletonMain &) = delete;
		~SkeletonMain();

		GameObjectAnim *GetFirstBone();

	private:
		void privSetAnimationHierarchy(Bone *pBoneResult);
		GameObjectAnim *privFindBoneByIndex(int index);

	private:
		GameObjectAnim *pFirstBone;
		int             numBones;
	};
}

#endif

// --- End of File ---
