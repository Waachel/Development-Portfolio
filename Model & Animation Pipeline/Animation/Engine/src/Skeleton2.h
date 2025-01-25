//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Skeleton2_H
#define Skeleton2_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton2
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton2(Bone *pBone, int numBones);

		Skeleton2() = delete;
		Skeleton2(const Skeleton2 &) = delete;
		Skeleton2 &operator = (const Skeleton2 &) = delete;
		~Skeleton2();

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
