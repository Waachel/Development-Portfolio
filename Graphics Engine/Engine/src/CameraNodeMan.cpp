//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraNodeMan.h"
#include "CameraNull.h"
#include "ManBase.h"
#include "DLinkMan.h"

CameraNodeMan *CameraNodeMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
CameraNodeMan::CameraNodeMan(int reserveNum, int reserveGrow)
	: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
	// Preload the reserve
	this->proFillReservedPool(reserveNum);

	// initialize derived data here
	Camera *pCamera = new CameraNull();
	assert(pCamera);

	this->poNodeCompare = new CameraNode();
	assert(this->poNodeCompare);

	this->poNodeCompare->Set(pCamera);
}

CameraNodeMan::~CameraNodeMan()
{
	delete this->poNodeCompare;
	this->poNodeCompare = nullptr;

	// iterate through the list and delete
	Iterator *pIt = this->baseGetActiveIterator();

	DLink *pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		CameraNode *pDeleteMe = (CameraNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}

	pIt = this->baseGetReserveIterator();

	pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		CameraNode *pDeleteMe = (CameraNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void CameraNodeMan::Create(int reserveNum, int reserveGrow)
{
	// make sure values are ressonable 
	assert(reserveNum >= 0);
	assert(reserveGrow > 0);

	// initialize the singleton here
	assert(posInstance == nullptr);

	// Do the initialization
	if (posInstance == nullptr)
	{
		posInstance = new CameraNodeMan(reserveNum, reserveGrow);
	}
}

void CameraNodeMan::Destroy()
{
	CameraNodeMan *pMan = CameraNodeMan::privGetInstance();
	assert(pMan != nullptr);
	AZUL_UNUSED_VAR(pMan);

	delete CameraNodeMan::posInstance;
	CameraNodeMan::posInstance = nullptr;
}

CameraNode *CameraNodeMan::Add(Camera *pMesh)
{
	CameraNodeMan *pMan = CameraNodeMan::privGetInstance();

	assert(pMesh);

	CameraNode *pNode = (CameraNode *)pMan->baseAddToFront();
	assert(pNode != nullptr);

	// Initialize the date
	pNode->Set(pMesh);

	return pNode;
}

CameraNode *CameraNodeMan::Find(Camera::Name _name)
{
	CameraNodeMan *pMan = CameraNodeMan::privGetInstance();
	assert(pMan != nullptr);

	// Compare functions only compares two Nodes

	// So:  Use the Compare Node - as a reference
	//      use in the Compare() function
	Camera *pCamera = pMan->poNodeCompare->GetCamera();
	assert(pCamera);

	pCamera->name = _name;

	CameraNode *pData = (CameraNode *)pMan->baseFind(pMan->poNodeCompare);
	return pData;
}

void CameraNodeMan::Remove(CameraNode *pNode)
{
	assert(pNode != nullptr);

	CameraNodeMan *pMan = CameraNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseRemove(pNode);
}

void CameraNodeMan::Dump()
{
	CameraNodeMan *pMan = CameraNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseDump();
}

void CameraNodeMan::SetCurrentCam(const Camera::Name name)
{
	// Get the instance to the manager
	CameraNodeMan* pCameraMan = CameraNodeMan::privGetInstance();
	assert(pCameraMan);

	CameraNode* pCam = CameraNodeMan::Find(name);

	pCameraMan->activeCam = pCam->poCamera;
}

void CameraNodeMan::Update()
{
	// Get the instance to the manager
	CameraNodeMan* pCameraMan = CameraNodeMan::privGetInstance();
	assert(pCameraMan);

	pCameraMan->activeCam->updateCamera();
}

Camera* CameraNodeMan::GetActiveCam()
{
	// Get the instance to the manager
	CameraNodeMan* pCameraMan = CameraNodeMan::privGetInstance();
	assert(pCameraMan);

	return pCameraMan->activeCam;
}

void CameraNodeMan::GetActiveHelper(Vec3& up, Vec3& tar, Vec3& pos, Vec3& upNorm, Vec3& forwardNorm, Vec3& rightNorm)
{
	// Get the instance to the manager
	CameraNodeMan* pCameraMan = CameraNodeMan::privGetInstance();
	assert(pCameraMan);

	pCameraMan->activeCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
}

void CameraNodeMan::SetActiveHelper(Vec3& up, Vec3& tar, Vec3& pos)
{
	// Get the instance to the manager
	CameraNodeMan* pCameraMan = CameraNodeMan::privGetInstance();
	assert(pCameraMan);

	pCameraMan->activeCam->SetHelper(up, tar, pos);
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
CameraNodeMan *CameraNodeMan::privGetInstance()
{
	// Safety - this forces users to call Create() first before using class
	assert(posInstance != nullptr);

	return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *CameraNodeMan::derivedCreateNode()
{
	DLink *pNodeBase = new CameraNode();
	assert(pNodeBase != nullptr);

	return pNodeBase;
}


// --- End of File ---




