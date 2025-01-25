//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"
#include "CameraMan.h"

namespace Azul
{

	GameObject::GameObject(GraphicsObject *pGraphicsObject)
		: poPos{new Vec3(0,0,0)},
		poGraphicsObject(pGraphicsObject),
		scale(1.0f)
	{
		assert(pGraphicsObject);
		assert(poPos);

		this->poWorld = new Mat4(Special::Identity);
		assert(this->poWorld);

	}
	void GameObject::SetScale(float v)
	{
		this->scale = v;
	}

	GameObject::~GameObject()
	{
		delete this->poPos;
		delete this->poGraphicsObject;
		delete this->poWorld;
	}

	Mat4 *GameObject::GetWorld()
	{
		return this->poWorld;
	}

	void GameObject::SetPos(Vec3 v)
	{
		poPos->set(v);
	}

	void GameObject::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		static float angle = 0.0f;
		angle += 0.005f;
		Trans T(poPos->x(), poPos->y(), poPos->z());
		Scale S(scale, scale, scale);
		Rot R(Rot1::Y, angle);
		*this->poWorld = S * R * T;

		//this->poWorld->Print("gameobjec: world");
		// PUSH to Graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObject::SetWorld(Mat4 *world)
	{
		assert(world);
		*this->poWorld = *world;
	}


	void GameObject::Draw()
	{
		// ---------------------------------------------
		//  Transfer data to the constant buffer
		//    CPU ---> GPU
		//    World, View, Projection Matrix
		//    Set Shader
		//    Render
		// ---------------------------------------------
		this->poGraphicsObject->Render();

	}

}

// --- End of File ---
