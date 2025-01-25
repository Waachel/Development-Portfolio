//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Skeleton3_H
#define Skeleton3_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton3
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton3(Bone *pBone, int numBones);

		Skeleton3() = delete;
		Skeleton3(const Skeleton3 &) = delete;
		Skeleton3 &operator = (const Skeleton3 &) = delete;
		~Skeleton3();

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
