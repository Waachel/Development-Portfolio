//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_LAYOUT_H
#define MESH_LAYOUT_H

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
		: x(0.0f), y(0.0f), z(0.0f){}
	Vec3f(const Vec3f &) = default;
	Vec3f &operator = (const Vec3f &) = default;
	~Vec3f() = default;

	Vec3f(const float a, const float b, const float c)
		: x(a), y(b), z(c){}

	Vec3f operator * (float val)
	{
		x *= val;
		y *= val;
		z *= val;

		return *this;
	}

	float x;
	float y;
	float z;
};

struct Vec4f
{
	Vec4f()
		: x(0.0f), y(0.0f), z(0.0f), a(1.0f){}
	Vec4f(const Vec4f &) = default;
	Vec4f &operator = (const Vec4f &) = default;
	~Vec4f() = default;

	Vec4f(const float a, const float b, const float c, const float d)
		: x(a), y(b), z(c), a(d){}


	float x;
	float y;
	float z;
	float a;
};

struct Vec3i
{
	Vec3i() = default;
	Vec3i(const Vec3i &) = default;
	Vec3i &operator = (const Vec3i &) = default;
	~Vec3i() = default;

	Vec3i(const unsigned int a, const unsigned int b, const unsigned int c)
		: i0(a), i1(b), i2(c){}


	unsigned int i0;
	unsigned int i1;
	unsigned int i2;
};

// Vertex data for a colored cube.
struct VertexColor
{
	Vec4f Color;  // r,g,b,a
};

struct VertexPos
{
	VertexPos() = default;
	VertexPos(const VertexPos &) = default;
	VertexPos &operator = (const VertexPos &) = default;
	~VertexPos() = default;

	Vec3f Position;
};

struct VertexNorm
{
	Vec3f Norm;
};

struct VertexTexCoord
{
	Vec2f TexCoord;
};

struct TriIndex
{
	Vec3i index;
};


#endif

// --- End of File ---
