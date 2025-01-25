//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include "Model.h"

class CubeModel : public Model
{
public:
	CubeModel();
	CubeModel(const CubeModel &) = delete;
	CubeModel &operator = (CubeModel &) = delete;
	virtual ~CubeModel();


	// Data

};

#endif

// --- End of File ---