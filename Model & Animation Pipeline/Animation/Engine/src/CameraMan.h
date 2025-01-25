//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAM_NODE_MAN_H
#define CAM_NODE_MAN_H 

#include "ManBase.h"
#include "CameraNode.h"
#include "Camera.h"

namespace Azul
{
	class CameraMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static CameraNode *Add(Camera::Name name, Camera *pCam);
		static Camera *Find(Camera::Name name);

		static void SetCurrent(Camera::Name name);
		static Camera *GetCurrent();

		static void Remove(CameraNode *pNode);
		static void Dump();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static CameraMan *privGetInstance();

		CameraMan() = delete;
		CameraMan(const CameraMan &) = delete;
		CameraMan &operator = (const CameraMan &) = delete;
		~CameraMan();

		CameraMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		CameraNode *poNodeCompare;
		static CameraMan *posInstance;

		Camera *currCamera;

	};
}

#endif

// --- End of File ---
