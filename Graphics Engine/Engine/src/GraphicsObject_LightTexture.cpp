//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_LightTexture.h"
#include <CameraNodeMan.h>

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;
extern Camera *pHackCamera;

using namespace Azul;

// ---------------------------------------------
//  Transfer data to the constant buffer
//    CPU ---> GPU
//    World, View, Projection Matrix
// ---------------------------------------------
GraphicsObject_LightTexture::GraphicsObject_LightTexture(Mesh *mesh,
														 ShaderObject *pShaderObj,
														 TextureObject *_pTex,
														 Vec3 &LightColor,
														 Vec3 &LightPos)
	: GraphicsObject(mesh, pShaderObj),
	pTex(_pTex),
	poLightColor(nullptr),
	poLightPos(nullptr)
{
	assert(pTex);

	poLightColor = new Vec3(LightColor);
	poLightPos = new Vec3(LightPos);

	assert(poLightColor);
	assert(poLightPos);
}

GraphicsObject_LightTexture::~GraphicsObject_LightTexture()
{
	delete poLightColor;
	delete poLightPos;
}

void GraphicsObject_LightTexture::SetState()
{
	// Future - settings to directX
	// say make it wireframe or change culling mode
	this->pTex->ActivateTexture();
}

void GraphicsObject_LightTexture::SetDataGPU()
{
	pShaderObj->ActivateShader();
	pMesh->ActivateMesh();
	pHackCamera = CameraNodeMan::GetActiveCam();
	pMesh->TransferConstantBuffer(pHackCamera, this->poWorld);

	pHackDeviceContext->UpdateSubresource(pMesh->poConstantBuff_lightColor, 0, nullptr, poLightColor, 0, 0);
	pHackDeviceContext->UpdateSubresource(pMesh->poConstantBuff_lightPos, 0, nullptr, poLightPos, 0, 0);


}

void GraphicsObject_LightTexture::Draw()
{
	pMesh->RenderIndexBuffer();
}

void GraphicsObject_LightTexture::RestoreState()
{
	// Future - Undo settings to directX
}


// --- End of File ---
