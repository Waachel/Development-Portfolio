//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "TextureProto.h"
#include "File.h"
#include "meshData.h"

#include "Engine.h"
#include "StringThis.h"

#include <Windows.h>
#include <string>
#include <d3d11.h>
#include "DirectXTex.h"
#include "DirectXDeviceMan.h"

namespace Azul
{
	TextureProto::TextureProto(const char *const pMeshFileName,
							   D3D11_FILTER filter)
		: TextureObject(),
		poTex(nullptr)
	{
		// Fill in textTexture
		DirectX::ScratchImage testTexture;

		// ----------------------------------------------
		// READ the data from the file ONLY
		// ----------------------------------------------

		// File stuff
		File::Handle fh;
		File::Error  ferror;

		ferror = File::Open(fh, pMeshFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Get the size
		ferror = File::Seek(fh, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		DWORD length;
		ferror = File::Tell(fh, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		char *poBuff = new char[length];
		assert(poBuff);

		ferror = File::Read(fh, poBuff, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		std::string strIn(poBuff, length);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);

		delete[] poBuff;

		//----------------------

		// Create texture
		HRESULT hr;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mB.text_color.width;
		desc.Height = mB.text_color.height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;

		assert((mB.text_color.textType == textureData::TEXTURE_TYPE::PNG)
			   || (mB.text_color.textType == textureData::TEXTURE_TYPE::TGA));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //GUID_WICPixelFormat32bppRGBA

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = mB.text_color.poData;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;

		hr = DirectXDeviceMan::GetDevice()->CreateTexture2D(&desc, &subResource, &poTex);

		//----------------------

		if(SUCCEEDED(hr) && poTex != nullptr)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			memset(&SRVDesc, 0, sizeof(SRVDesc));
			SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = 1;

			hr = DirectXDeviceMan::GetDevice()->CreateShaderResourceView(poTex, &SRVDesc, &poTextureRV);
			assert(SUCCEEDED(hr));
		}
		else
		{
			assert(false);
		}

		//-----------------------------------

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = filter;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = DirectXDeviceMan::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);
		assert(SUCCEEDED(hr));
	}


	TextureProto::~TextureProto()
	{
		SafeRelease(poTex);
	}

}

// --- End of File ---
