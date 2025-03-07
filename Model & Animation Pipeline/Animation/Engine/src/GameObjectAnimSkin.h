//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKIN_H
#define GAME_OBJECT_ANIM_SKIN_H

#include "GameObjectAnim.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
	class GameObjectAnimSkin : public GameObjectAnim
	{
	public:
		GameObjectAnimSkin(GraphicsObject *graphicsObject, Bone *pBoneResult);

		// Big four
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin &) = delete;
		GameObjectAnimSkin &operator=(GameObjectAnimSkin &) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(Vec3 &r);
		void SetQuat(Quat &r);
		void SetTrans(Vec3 &r);

		virtual void SetIndex(int i) override;

		Mat4 GetBoneOrientation(void) const;
		void SetBoneOrientation(const Mat4 &);

	private:
		void privUpdate(AnimTime currTime);

	public:   // add accessors later
		Vec3 *poScale;
		Quat *poQuat;
		Vec3 *poTrans;
		Mat4 *poLocal;
	//	int index;
		Bone *pBoneResult;
		Mat4 *poBoneOrientation;
	};
}

#endif

// --- End of File ---
