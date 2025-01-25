//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TextureObjectNodeMan.h"
#include "TextureObject_Null.h"
#include "ManBase.h"
#include "DLinkMan.h"

TextureObjectNodeMan *TextureObjectNodeMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TextureObjectNodeMan::TextureObjectNodeMan(int reserveNum, int reserveGrow)
	: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
	// Preload the reserve
	this->proFillReservedPool(reserveNum);

	// initialize derived data here
	TextureObject *pTextureObject = new TextureObject_Null();
	assert(pTextureObject);

	this->poNodeCompare = new TextureObjectNode();
	assert(this->poNodeCompare);

	this->poNodeCompare->Set(pTextureObject);
}

TextureObjectNodeMan::~TextureObjectNodeMan()
{
	delete this->poNodeCompare;
	this->poNodeCompare = nullptr;

	// iterate through the list and delete
	Iterator *pIt = this->baseGetActiveIterator();

	DLink *pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		TextureObjectNode *pDeleteMe = (TextureObjectNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}

	pIt = this->baseGetReserveIterator();

	pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		TextureObjectNode *pDeleteMe = (TextureObjectNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void TextureObjectNodeMan::Create(int reserveNum, int reserveGrow)
{
	// make sure values are ressonable 
	assert(reserveNum >= 0);
	assert(reserveGrow > 0);

	// initialize the singleton here
	assert(posInstance == nullptr);

	// Do the initialization
	if (posInstance == nullptr)
	{
		posInstance = new TextureObjectNodeMan(reserveNum, reserveGrow);
	}
}

void TextureObjectNodeMan::Destroy()
{
	TextureObjectNodeMan *pMan = TextureObjectNodeMan::privGetInstance();
	assert(pMan != nullptr);
	AZUL_UNUSED_VAR(pMan);

	delete TextureObjectNodeMan::posInstance;
	TextureObjectNodeMan::posInstance = nullptr;
}

TextureObjectNode *TextureObjectNodeMan::Add(TextureObject *pMesh)
{
	TextureObjectNodeMan *pMan = TextureObjectNodeMan::privGetInstance();

	assert(pMesh);

	TextureObjectNode *pNode = (TextureObjectNode *)pMan->baseAddToFront();
	assert(pNode != nullptr);

	// Initialize the date
	pNode->Set(pMesh);

	return pNode;
}

TextureObjectNode *TextureObjectNodeMan::Find(TextureObject::Name _name)
{
	TextureObjectNodeMan *pMan = TextureObjectNodeMan::privGetInstance();
	assert(pMan != nullptr);

	// Compare functions only compares two Nodes

	// So:  Use the Compare Node - as a reference
	//      use in the Compare() function
	TextureObject *pTextureObject = pMan->poNodeCompare->GetTextureObject();
	assert(pTextureObject);

	pTextureObject->name = _name;

	TextureObjectNode *pData = (TextureObjectNode *)pMan->baseFind(pMan->poNodeCompare);
	return pData;
}

void TextureObjectNodeMan::Remove(TextureObjectNode *pNode)
{
	assert(pNode != nullptr);

	TextureObjectNodeMan *pMan = TextureObjectNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseRemove(pNode);
}

void TextureObjectNodeMan::Dump()
{
	TextureObjectNodeMan *pMan = TextureObjectNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
TextureObjectNodeMan *TextureObjectNodeMan::privGetInstance()
{
	// Safety - this forces users to call Create() first before using class
	assert(posInstance != nullptr);

	return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *TextureObjectNodeMan::derivedCreateNode()
{
	DLink *pNodeBase = new TextureObjectNode();
	assert(pNodeBase != nullptr);

	return pNodeBase;
}


// --- End of File ---




