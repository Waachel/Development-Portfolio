//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef TEXTURE_OBJECT_NULL_H
#define TEXTURE_OBJECT_NULL_H

#include "TextureObject.h"

class TextureObject_Null : public TextureObject
{
public:

public:
	TextureObject_Null();
	TextureObject_Null(const TextureObject_Null &) = delete;
	TextureObject_Null &operator = (const TextureObject_Null &) = delete;
	virtual ~TextureObject_Null();


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---