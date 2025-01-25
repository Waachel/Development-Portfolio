//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SOLID_H
#define GRAPHICS_OBJECT_SOLID_H

#include "GraphicsObject.h"

class GraphicsObject_Solid :public GraphicsObject
{
public:
	GraphicsObject_Solid(Mesh *mesh, ShaderObject *pShaderObj);

	GraphicsObject_Solid() = delete;
	GraphicsObject_Solid(const GraphicsObject_Solid &) = delete;
	GraphicsObject_Solid &operator = (const GraphicsObject_Solid &) = delete;
	virtual ~GraphicsObject_Solid() = default;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

};

#endif

// ---  End of File ---