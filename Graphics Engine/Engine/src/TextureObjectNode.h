//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_OBJECT_NODE_H
#define TEXTURE_OBJECT_NODE_H

#include "DLink.h"
#include "TextureObject.h"

class TextureObjectNode : public DLink
{
public:
	TextureObjectNode();
	TextureObjectNode(const TextureObject &) = delete;
	TextureObjectNode &operator = (const TextureObject &) = delete;
	virtual ~TextureObjectNode();

	void Set(TextureObject *pTextureObject);
	TextureObject *GetTextureObject();

	virtual char *GetName() override;
	virtual void Wash() override;
	virtual bool Compare(DLink *pTargetNode) override;
	virtual void Dump() override;

private:
	void privClear();

	//--------------------------------------------------
	// Data
	//--------------------------------------------------
	TextureObject *poTextureObject;
};

#endif

// --- End of File ---
