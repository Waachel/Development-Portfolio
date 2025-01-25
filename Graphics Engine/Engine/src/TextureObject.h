//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include <d3d11.h>

class TextureObject
{
public:
	enum class Name
	{
		NullTexture,
		Duckweed,
		Brick,
		Stone,
		Rocks,
		Uninitialized
	};


public:
	TextureObject();
	TextureObject(const TextureObject &) = delete;
	TextureObject &operator = (const TextureObject &) = delete;
	virtual ~TextureObject();

	TextureObject(TextureObject::Name _name,
				  LPCWSTR filepath,
				  D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	void ActivateTexture();

	char *GetName();
	void SetName(TextureObject::Name _name);

	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------
	TextureObject::Name name;

	int TexResourceSlot;
	int SamplerSlot;

	ID3D11ShaderResourceView *poTextureRV;
	ID3D11SamplerState *poSampler;

};

#endif

// --- End of File ---
