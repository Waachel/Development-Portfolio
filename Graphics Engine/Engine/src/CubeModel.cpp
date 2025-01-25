//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CubeModel.h"
#include "MathEngine.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

using namespace Azul;


VertexPos g_Vertices_pos[] =
{
	Vec3(-1.0f, -1.0f, -1.0f), // 0
	Vec3(-1.0f,  1.0f, -1.0f), // 1
	Vec3(1.0f,  1.0f, -1.0f) , // 2
	Vec3(1.0f, -1.0f, -1.0f) , // 3
	Vec3(-1.0f, -1.0f,  1.0f), // 4
	Vec3(-1.0f,  1.0f,  1.0f), // 5
	Vec3(1.0f,  1.0f,  1.0f) , // 6
	Vec3(1.0f, -1.0f,  1.0f)   // 7
};

VertexColor g_Vertices_color[] =
{
	 Vec3(0.0f, 0.0f, 0.0f) , // 0
	 Vec3(0.0f, 1.0f, 0.0f) , // 1
	 Vec3(1.0f, 1.0f, 0.0f) , // 2
	 Vec3(1.0f, 0.0f, 0.0f) , // 3
	 Vec3(0.0f, 0.0f, 1.0f) , // 4
	 Vec3(0.0f, 1.0f, 1.0f) , // 5
	 Vec3(1.0f, 1.0f, 1.0f) , // 6
	 Vec3(1.0f, 0.0f, 1.0f)   // 7
};


uint32_t g_Indicies[] =
{
	// Triangle indices... 
	// 12 triangles, 12 * 3 verts/tri = 36 indices
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	4, 7, 6,
	4, 5, 1,
	4, 1, 0,
	3, 2, 6,
	3, 6, 7,
	1, 5, 6,
	1, 6, 2,
	4, 0, 3,
	4, 3, 7
};
unsigned int NumVerts = sizeof(g_Vertices_pos) / sizeof(g_Vertices_pos[0]);
unsigned int NumIndices = sizeof(g_Indicies) / sizeof(g_Indicies[0]);


CubeModel::CubeModel()
	: Model(NumVerts, NumIndices)
{
	// Create an initialize the vertex buffer.
	poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
	poVertexBuffer_color = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);

	// Create and initialize the index buffer.
	poIndexBuffer = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);

	// Create the constant buffers for the variables defined in the vertex shader.
	poConstantBuff_Projection = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_View = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_World = CreateConstantBuffer(sizeof(Mat4));
}


CubeModel::~CubeModel()
{
	// remove anything dynamic here
}


// --- End of File ---