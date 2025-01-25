//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_Diamond_H
#define MESH_Diamond_H

#include "Mesh.h"

class MeshDiamond : public Mesh
{
public:
	MeshDiamond();
	MeshDiamond(const MeshDiamond &) = delete;
	MeshDiamond &operator = (MeshDiamond &) = delete;
	virtual ~MeshDiamond() = default;


	// Data

};

#endif

// --- End of File ---
