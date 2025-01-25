//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshCross.h"
#include "MathEngine.h"
#include "Engine.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

using namespace Azul;

VertexPos g_CrossVertices_pos[] =
{
		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.35f,  0.05f),
		Vec3(-0.05f,  0.35f,  -0.05f), 
		Vec3(0.05f,  0.35f,  -0.05f),

		Vec3(-0.05f,  0.05f,  0.05f),	
		Vec3(0.05f,  0.05f,  0.05f),	
		Vec3(-0.05f,  0.05f,  -0.05f),	
		Vec3(0.05f,  0.05f,  -0.05f),	

		Vec3(-0.05f,  -0.05f,  0.05f),	
		Vec3(0.05f,  -0.05f,  0.05f),	
		Vec3(-0.05f,  -0.05f,  -0.05f),
		Vec3(0.05f,  -0.05f,  -0.05f),	


		Vec3(-0.05f,  -0.35f,  0.05f), 
		Vec3(0.05f,  -0.35f,  0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f), 

		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  0.05f), 
		Vec3(-0.35f,  0.05f,  -0.05f), 
		Vec3(-0.35f,  -0.05f,  -0.05f),

		Vec3(0.35f,  0.05f,  0.05f), 
		Vec3(0.35f,  -0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  -0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),

		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.35f,  0.05f), 
		Vec3(0.05f,  0.35f,  0.05f), 
		Vec3(0.05f,  0.05f,  0.05f), 
		Vec3(0.05f,  0.35f,  -0.05f),
		Vec3(-0.05f,  0.35f,  -0.05f),
		Vec3(0.05f,  0.05f,  -0.05f),
		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(-0.05f,  0.05f,  0.05f),
		Vec3(-0.05f,  0.05f,  -0.05f),
		Vec3(0.35f,  0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  -0.05f),
		Vec3(-0.35f,  -0.05f,  -0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  0.05f),
		Vec3(0.35f,  -0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),
		Vec3(0.05f,  -0.35f,  0.05f),
		Vec3(0.05f,  -0.05f,  0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.05f,  -0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f),
		Vec3(-0.05f,  -0.05f,  -0.05f),
		Vec3(-0.05f,  -0.05f,  0.05f),
		Vec3(-0.05f,  -0.35f,  0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f)
};

VertexColor g_CrossVertices_color[] =
{
		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.35f,  0.05f),
		Vec3(-0.05f,  0.35f,  -0.05f),
		Vec3(0.05f,  0.35f,  -0.05f),

		Vec3(-0.05f,  0.05f,  0.05f),
		Vec3(0.05f,  0.05f,  0.05f),
		Vec3(-0.05f,  0.05f,  -0.05f),
		Vec3(0.05f,  0.05f,  -0.05f),

		Vec3(-0.05f,  -0.05f,  0.05f),
		Vec3(0.05f,  -0.05f,  0.05f),
		Vec3(-0.05f,  -0.05f,  -0.05f),
		Vec3(0.05f,  -0.05f,  -0.05f),


		Vec3(-0.05f,  -0.35f,  0.05f),
		Vec3(0.05f,  -0.35f,  0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f),

		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  0.05f),
		Vec3(-0.35f,  0.05f,  -0.05f),
		Vec3(-0.35f,  -0.05f,  -0.05f),

		Vec3(0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  -0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  -0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),

		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.35f,  0.05f),
		Vec3(0.05f,  0.05f,  0.05f),
		Vec3(0.05f,  0.35f,  -0.05f),
		Vec3(-0.05f,  0.35f,  -0.05f),
		Vec3(0.05f,  0.05f,  -0.05f),
		Vec3(-0.05f,  0.35f,  0.05f),
		Vec3(-0.05f,  0.05f,  0.05f),
		Vec3(-0.05f,  0.05f,  -0.05f),
		Vec3(0.35f,  0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  -0.05f),
		Vec3(-0.35f,  -0.05f,  -0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  0.05f),
		Vec3(0.35f,  -0.05f,  0.05f),
		Vec3(-0.35f,  -0.05f,  -0.05f),
		Vec3(-0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  0.05f,  0.05f),
		Vec3(0.35f,  -0.05f,  -0.05f),
		Vec3(0.05f,  -0.35f,  0.05f),
		Vec3(0.05f,  -0.05f,  0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.35f,  -0.05f),
		Vec3(0.05f,  -0.05f,  -0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f),
		Vec3(-0.05f,  -0.05f,  -0.05f),
		Vec3(-0.05f,  -0.05f,  0.05f),
		Vec3(-0.05f,  -0.35f,  0.05f),
		Vec3(-0.05f,  -0.35f,  -0.05f)
};


uint32_t g_CrossIndices[] =
{
		0,2,1,
		2,3,1,
		24,5,4,
		24,25,5,
		26,7,27,
		26,3,7,
		28,29,30,
		29,6,30,
		2,31,32,
		2,32,33,
		22,20,16,
		22,16,18,
		19,34,35,
		23,34,19,
		36,17,37,
		17,21,37,
		38,39,40,
		39,41,40,
		42,18,17,
		18,43,17,
		21,44,45,
		44,22,45,
		14,12,15,
		12,13,15,
		12,8,9,
		9,13,12,
		46,47,11,
		11,48,46,
		49,50,10,
		10,51,49,
		52,53,54,
		55,52,54
};

VertexTexCoord g_CrossVertices_texCoord[] =
{
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),

		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 1.0f),

		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 0.0f),


		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),

		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),

		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),

		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f)

};

VertexNorm g_CrossVertices_norm[] =
{
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),


		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f)
};

unsigned int xxxxxx = sizeof(g_CrossVertices_pos);
unsigned int NumCrossVerts = sizeof(g_CrossVertices_pos) / sizeof(g_CrossVertices_pos[0]);
unsigned int NumCrossIndices = sizeof(g_CrossIndices) / sizeof(g_CrossIndices[0]);


MeshCross::MeshCross()
	: Mesh(NumCrossVerts, NumCrossIndices)
{
	// Create an initialize the vertex buffer.
	poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_CrossVertices_pos), g_CrossVertices_pos);
	poVertexBuffer_color = CreateVertexBuffer(sizeof(g_CrossVertices_color), g_CrossVertices_color);
	poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_CrossVertices_texCoord), g_CrossVertices_texCoord);
	poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_CrossVertices_norm), g_CrossVertices_norm);

	// Create and initialize the index buffer.
	poIndexBuffer = CreateIndexBuffer(sizeof(g_CrossIndices), g_CrossIndices);

	// Create the constant buffers for the variables defined in the vertex shader.
	poConstantBuff_Projection = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_View = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_World = CreateConstantBuffer(sizeof(Mat4));

	// Additional material owned by meshCross
	poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
	poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));
}


// --- End of File ---
