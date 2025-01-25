//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_NULL_H
#define MODEL_NULL_H
	
#include "Model.h"

class ModelNull : public Model
{
public:
	ModelNull();
	ModelNull(const ModelNull &) = delete;
	ModelNull &operator=(const ModelNull &other) = delete;
	virtual ~ModelNull();


private:

};

#endif

// --- End of File ---