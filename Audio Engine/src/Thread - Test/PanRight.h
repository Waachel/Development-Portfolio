//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PanRight_H
#define PanRight_H


#include "Snd.h"

class PanRight
{

public:
	// Big 4
	PanRight() = default;
	PanRight(const PanRight&) = default;
	PanRight& operator = (const PanRight&) = default;
	~PanRight() = default;

	void PanRight_Main();
	void PanRight_Stop();

public:
	// Data

};

#endif

// End of File 