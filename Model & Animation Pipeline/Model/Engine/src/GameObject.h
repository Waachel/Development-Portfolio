//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <d3d11.h>

#include "PCSNode.h"
#include "MathEngine.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "GraphicsObject.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObject : public PCSNode
	{
	public:
		GameObject() = delete;
		GameObject(const GameObject &) = delete;
		GameObject &operator = (const GameObject &) = delete;
		virtual ~GameObject();

		GameObject(GraphicsObject *poGraphicsObject);

		void Update(AnimTime currentTime);
		void Draw();

		Mat4 *GetWorld();
		void SetWorld(Mat4 *world);
		void SetPos(Vec3 v);
		void SetScale(float v);
	private:
		float scale;
		Vec3 *poPos;
		Mat4 *poWorld;
		GraphicsObject *poGraphicsObject;
	};

}
#endif


// --- End of File ---
