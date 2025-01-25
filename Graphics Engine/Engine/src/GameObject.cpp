//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;
extern Camera *pHackCamera;

namespace Azul
{

	GameObject::GameObject(GraphicsObject *pGraphicsObject)
		: poPos{new Vec3(0,0,0)},
		poGraphicsObject(pGraphicsObject),
		scale(1.0f)
	{
	assert(pGraphicsObject);
	assert(poPos);

	this->poWorld = new Mat4(Mat4::Special::Identity);
	//default - do nothing
	this->rotation = false;
	this->changeX = false;
	this->changeY = false;
	this->changeZ = false;
	assert(this->poWorld);

	}
	void GameObject::SetScale(float v)
	{
		this->scale = v;
	}

	void GameObject::SetRot(Mat4::Rot1 axis, float _angle)
	{
		this->rotation = true;
		this->rotAxis = axis;
		this->rotAngle = _angle;
	}

	void GameObject::SetDeltaX(float x)
	{
		this->changeX = true;
		this->deltaX = x;
	}

	void GameObject::SetDeltaY(float y)
	{
		this->changeY = true;
		this->deltaY = y;
	}

	void GameObject::SetDeltaZ(float z)
	{
		this->changeZ = true;
		this->deltaZ = z;
	}

	void GameObject::SetDeltaRot(float dRot)
	{
		this->deltaRot = dRot;
	}

	void GameObject::privUpdate(float currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);
		Mat4 Scale(Mat4::Scale::XYZ, scale, scale, scale);
		float x = poPos->x();
		float y = poPos->y();
		float z = poPos->z();

		if (changeX == true)
		{
			x = deltaX * cosf(currentTime) + 2;
		}
		if (changeY == true)
		{
			y = deltaY * cosf(currentTime) + 2;
		}
		if (changeZ == true)
		{
			z = deltaZ * cosf(currentTime) + 2;
		}
		Mat4 Trans(Mat4::Trans::XYZ, x, y, z);

		if (rotation == true)
		{
			this->rotAngle += this->deltaRot;
			Mat4 Rotation(this->rotAxis, this->rotAngle);

			*this->poWorld = Scale * Trans * Rotation;
		}

		else
		{
			*this->poWorld = Scale * Trans;
		}
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

	void GameObject::Update(float currentTime)
	{
		// Update the world matrix
		this->privUpdate(currentTime);

		// Push to graphics object
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
