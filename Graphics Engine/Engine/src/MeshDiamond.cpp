//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshDiamond.h"
#include "MathEngine.h"
#include "Engine.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

using namespace Azul;

VertexPos g_DiamondVertices_pos[] =
{
		Vec3(0.0f,  0.25f,  0.0f),
		Vec3(0.25f,  -0.25f,  0.25f), 
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(0.0f, -0.75f,  0.0f),
		Vec3(0.25f,  -0.25f,  0.25f), 
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f) 
};

VertexColor g_DiamondVertices_color[] =
{
		Vec3(0.0f,  0.25f,  0.0f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(0.0f, -0.75f,  0.0f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,  -0.25f,  0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(0.25f,   -0.25f, -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f, -0.25f,  -0.25f),
		Vec3(-0.25f,  -0.25f,  0.25f)
};


uint32_t g_DiamondIndices[] =
{
	0,1,2,
	0,3,6,
	0,4,7,
	0,8,9,
	10,11,5,
	12,13,5,
	14,15,5,
	16,17,5
};

VertexTexCoord g_DiamondVertices_texCoord[] =
{
	Vec2f(0.5f, 0.0f),
	Vec2f(1.0f, 1.0f),
	Vec2f(0.0f, 1.0f),
	Vec2f(1.0f, 1.0f),
	Vec2f(1.0f, 1.0f),

	Vec2f(0.5f, 1.0f),
	Vec2f(0.0f, 1.0f),
	Vec2f(0.0f, 1.0f),
	Vec2f(1.0f, 1.0f),
	Vec2f(0.0f, 1.0f),

	Vec2f(0.0f, 0.0f),
	Vec2f(1.0f, 0.0f),
	Vec2f(0.0f, 0.0f),
	Vec2f(1.0f, 0.0f),
	Vec2f(0.0f, 0.0f),

	Vec2f(1.0f, 0.0f),
	Vec2f(0.0f, 0.0f),
	Vec2f(1.0f, 0.0f)

};

VertexNorm g_DiamondVertices_norm[] =
{
	Vec3f(-0.6f,  0.6f, -0.6f),
	Vec3f(-0.6f, -0.6f, -0.6f),
	Vec3f(0.6f,  -0.6f, -0.6f),
	Vec3f(0.6f,  0.6f, -0.6f),
	Vec3f(0.6f, -0.6f,  0.6f),

	Vec3f(0.6f, -0.6f,  0.6f),
	Vec3f(-0.6f, -0.6f, -0.6f),
	Vec3f(0.6f,  0.6f, -0.6f),
	Vec3f(0.6f,  -0.6f, -0.6f),
	Vec3f(0.6f, -0.6f,  0.6f),

	Vec3f(0.6f,  -0.6f, -0.6f),
	Vec3f(-0.6f, -0.6f, -0.6f),
	Vec3f(-0.6f, -0.6f, -0.6f),
	Vec3f(0.6f,  0.6f, -0.6f),
	Vec3f(0.6f,  0.6f, -0.6f),

	Vec3f(0.6f, -0.6f,  0.6f),
	Vec3f(0.6f, -0.6f,  0.6f),
	Vec3f(0.6f,  -0.6f, -0.6f)
};

unsigned int xxxxx = sizeof(g_DiamondVertices_pos);
unsigned int NumDiamondVerts = sizeof(g_DiamondVertices_pos) / sizeof(g_DiamondVertices_pos[0]);
unsigned int NumDiamondIndices = sizeof(g_DiamondIndices) / sizeof(g_DiamondIndices[0]);


MeshDiamond::MeshDiamond()
	: Mesh(NumDiamondVerts, NumDiamondIndices)
{
	// Create an initialize the vertex buffer.
	poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_DiamondVertices_pos), g_DiamondVertices_pos);
	poVertexBuffer_color = CreateVertexBuffer(sizeof(g_DiamondVertices_color), g_DiamondVertices_color);
	poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_DiamondVertices_texCoord), g_DiamondVertices_texCoord);
	poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_DiamondVertices_norm), g_DiamondVertices_norm);

	// Create and initialize the index buffer.
	poIndexBuffer = CreateIndexBuffer(sizeof(g_DiamondIndices), g_DiamondIndices);

	// Create the constant buffers for the variables defined in the vertex shader.
	poConstantBuff_Projection = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_View = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_World = CreateConstantBuffer(sizeof(Mat4));

	// Additional material owned by meshDiamond
	poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
	poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));
}


// --- End of File ---
