//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Render.h"

namespace Azul
{
	class Mesh;

	class GraphicsObject : public Render
	{
	public:
		GraphicsObject(Mesh *mesh, ShaderObject *pShaderObj);

		GraphicsObject() = delete;
		GraphicsObject(const GraphicsObject &) = delete;
		GraphicsObject &operator = (const GraphicsObject &) = delete;
		virtual ~GraphicsObject() override;

		void Render();
		void SetWorld(Mat4 &_world);

		Mesh *GetMesh() const;
		Mat4 &GetWorld();

	protected:
		Mesh *pMesh;
		Mat4 *poWorld;
		ShaderObject *pShaderObj;
	};
}

#endif

// --- End of File ---
