//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include "DLink.h"
#include "MathEngine.h"
#include "Camera.h"

using namespace Azul;

// Quick Hack
struct Vec2f
{
	Vec2f()
		: u(0.0f), v(0.0f){}
	Vec2f(const Vec2f &) = default;
	Vec2f &operator = (const Vec2f &) = default;
	~Vec2f() = default;

	Vec2f(const float _u, const float _v)
		: u(_u), v(_v){}

	float u;
	float v;
};

struct Vec3f
{
	Vec3f()
		: x(0.0f),y(0.0f),z(0.0f){}
	Vec3f(const Vec3f &) = default;
	Vec3f &operator = (const Vec3f &) = default;
	~Vec3f() = default;

	Vec3f(const float a, const float b, const float c)
		: x(a), y(b), z(c){}

	float x;
	float y;
	float z;
};


// Vertex data for a colored cube.
struct VertexColor
{
	Vec3 Color;
};

struct VertexPos
{
	Vec3 Position;
};

struct VertexNorm
{
	Vec3f Norm;
};

struct VertexTexCoord
{
	Vec2f TexCoord;
};

enum class VertexSlot : uint32_t
{
	Position,  //0
	Color,     //1
	Norm,      //2
	TexCoord   //3
};

enum class ConstantBufferSlot : uint32_t
{
	Projection,
	View,
	World,

	Color,
	LightPos,
};

class Mesh
{
public:
	enum class Name
	{
		CUBE,
		PYRAMID,
		SPHERE,
		DIAMOND,
		CROSS,
		NULL_MESH,
		NOT_INITIALIZED
	};

public:
	Mesh() = delete;
	Mesh(Mesh &copyMesh) = delete;
	Mesh &operator = (Mesh &copyMesh) = delete;
	virtual ~Mesh();

	Mesh(unsigned int numVerts, unsigned int numTriangles);

	ID3D11Buffer *CreateVertexBuffer(unsigned int NumBytes, void *pData);
	ID3D11Buffer *CreateIndexBuffer(unsigned int NumBytes, void *pData);
	ID3D11Buffer *CreateConstantBuffer(unsigned int NumBytes);

	void Wash();
	bool Compare(DLink *pTarget);
	char *GetName();
	void SetName(Mesh::Name _name);

	void ActivateMesh();
	void TransferConstantBuffer(Camera *pCam, Mat4 *pWorld);
	void RenderIndexBuffer();

	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------

	Name name;

	unsigned int numVerts;
	unsigned int numIndices;

	// Vertex buffer data
	ID3D11Buffer *poVertexBuffer_pos;
	ID3D11Buffer *poVertexBuffer_color;
	ID3D11Buffer *poVertexBuffer_norm;
	ID3D11Buffer *poVertexBuffer_texCoord;

	ID3D11Buffer *poIndexBuffer;

	// Shader resources
	ID3D11Buffer *poConstantBuff_Projection;
	ID3D11Buffer *poConstantBuff_World;
	ID3D11Buffer *poConstantBuff_View;

	ID3D11Buffer *poConstantBuff_lightColor;
	ID3D11Buffer *poConstantBuff_lightPos;

};

#endif

// --- End of File ---
