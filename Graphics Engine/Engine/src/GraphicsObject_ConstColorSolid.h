//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_CONST_COLOR_SOLID_H
#define GRAPHICS_OBJECT_CONST_COLOR_SOLID_H

#include "GraphicsObject.h"
#include "TextureObject.h"

class GraphicsObject_ConstColorSolid :public GraphicsObject
{
public:
	GraphicsObject_ConstColorSolid(Mesh *mesh,
								ShaderObject *pShaderObj,
								Vec3 &_pLightColor);

	GraphicsObject_ConstColorSolid() = delete;
	GraphicsObject_ConstColorSolid(const GraphicsObject_ConstColorSolid &) = delete;
	GraphicsObject_ConstColorSolid &operator = (const GraphicsObject_ConstColorSolid &) = delete;
	virtual ~GraphicsObject_ConstColorSolid();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	Vec3 *poLightColor;

};
#endif


// --- End of File ---
