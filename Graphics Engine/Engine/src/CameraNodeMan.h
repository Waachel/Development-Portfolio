//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_NODE_MAN_H
#define CAMERA_NODE_MAN_H 

#include "ManBase.h"
#include "CameraNode.h"
#include "Camera.h"

class CameraNodeMan : public ManBase
{
	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
public:
	static void Create(int reserveNum = 0, int reserveGrow = 1);
	static void Destroy();

	static CameraNode *Add(Camera *pCamera);
	static CameraNode *Find(Camera::Name name);

	static void Remove(CameraNode *pNode);
	static void Dump();

	static void SetCurrentCam(const Camera::Name name);

	static void Update();

	static Camera* GetActiveCam();

	static void GetActiveHelper(Vec3& up, Vec3& tar, Vec3& pos, Vec3& upNorm, Vec3& forwardNorm, Vec3& rightNorm);

	static void SetActiveHelper(Vec3& up, Vec3& tar, Vec3& pos);

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
private:
	static CameraNodeMan *privGetInstance();

	~CameraNodeMan();
	CameraNodeMan(int reserveNum, int reserveGrow);

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
	static CameraNodeMan *posInstance;
	Camera* activeCam;

};

#endif

// --- End of File ---
