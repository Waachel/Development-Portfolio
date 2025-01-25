//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Skeleton1_H
#define Skeleton1_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton1
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton1(Bone *pBone, int numBones);

		Skeleton1() = delete;
		Skeleton1(const Skeleton1 &) = delete;
		Skeleton1 &operator = (const Skeleton1 &) = delete;
		~Skeleton1();

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
