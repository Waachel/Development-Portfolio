//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
	class Clip
	{
	public:

		enum class Type
		{
			WALK,
			RUN,
			DIE_LEFT,
			SIDE_STEP_RIGHT,
			NOT_INITIALIZED
		};


		struct FrameBucket
		{
			~FrameBucket()
			{
				delete[] poBone;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			AnimTime      KeyTime;
			Bone         *poBone;
			char          pad[4];
		};

	public:
		Clip(int _numFrames, int type);

		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		void ChangeAnim(int i);

		AnimTime GetTotalTime();
		void AnimateBones(AnimTime tCurr, Bone *pResult);

	private:
		void privSetAnimationData();
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

		void privWalkAnimation();
		void privDieLeftAnimation();
		void privSideStepRightAnimation();
		void privRunAnimation();

	private:
		int          numBones;
		int          numFrames;
		AnimTime     TotalTime;
		FrameBucket *poHead;

		int animType = 0;
	};
}

#endif

//--- End of File ---
