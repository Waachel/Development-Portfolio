//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Skeleton4_H
#define Skeleton4_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton4
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton4(Bone *pBone, int numBones);

		Skeleton4() = delete;
		Skeleton4(const Skeleton4 &) = delete;
		Skeleton4 &operator = (const Skeleton4 &) = delete;
		~Skeleton4();

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
