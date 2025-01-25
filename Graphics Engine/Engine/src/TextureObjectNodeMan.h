//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_OBJECT_NODE_MAN_H
#define TEXTURE_OBJECT_NODE_MAN_H 

#include "ManBase.h"
#include "TextureObjectNode.h"
#include "TextureObject.h"

class TextureObjectNodeMan : public ManBase
{
	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
public:
	static void Create(int reserveNum = 0, int reserveGrow = 1);
	static void Destroy();

	static TextureObjectNode *Add(TextureObject *pTextureObject);
	static TextureObjectNode *Find(TextureObject::Name name);

	static void Remove(TextureObjectNode *pNode);
	static void Dump();

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
private:
	static TextureObjectNodeMan *privGetInstance();

	~TextureObjectNodeMan();
	TextureObjectNodeMan(int reserveNum, int reserveGrow);

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
protected:
	DLink *derivedCreateNode() override;

	//----------------------------------------------------------------------
	// Data: unique data for this manager 
	//----------------------------------------------------------------------
private:
	TextureObjectNode *poNodeCompare;
	static TextureObjectNodeMan *posInstance;

};

#endif

// --- End of File ---
