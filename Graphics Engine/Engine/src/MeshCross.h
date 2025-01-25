//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_Cross_H
#define MESH_Cross_H

#include "Mesh.h"

class MeshCross : public Mesh
{
public:
	MeshCross();
	MeshCross(const MeshCross &) = delete;
	MeshCross &operator = (MeshCross &) = delete;
	virtual ~MeshCross() = default;


	// Data

};

#endif

// --- End of File ---
