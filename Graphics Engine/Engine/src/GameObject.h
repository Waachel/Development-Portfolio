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
#include <GameTimer.h>

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

	void Update(float currentTime);
	void Draw();

		Mat4 *GetWorld();
		void SetWorld(Mat4 *world);
	void SetPos(Vec3 v);
		void SetScale(float v);
		void SetRot(Mat4::Rot1, float _angle);
		void SetDeltaX(float x);
		void SetDeltaY(float y);
		void SetDeltaZ(float z);
		void SetDeltaRot(float dRot);
		void privUpdate(float currentTime);
	private:
		float scale;
		Mat4::Rot1 rotAxis;
		float rotAngle;
		float deltaRot;
		bool rotation;
		float deltaX;
		float deltaY;
		float deltaZ;
		bool changeX;
		bool changeY;
		bool changeZ;
		GameTimer mTimer;
	Vec3 *poPos;
	Mat4 *poWorld;
	GraphicsObject *poGraphicsObject;
	};

}
#endif


// --- End of File ---
