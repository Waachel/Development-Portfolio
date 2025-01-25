//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_SPHERE_H
#define MESH_SPHERE_H

#include "Mesh.h"

class MeshSphere : public Mesh
{
public:
	MeshSphere();
	MeshSphere(const MeshSphere &) = delete;
	MeshSphere &operator = (MeshSphere &) = delete;
	virtual ~MeshSphere() = default;


	// Data

};

#endif

// --- End of File ---
