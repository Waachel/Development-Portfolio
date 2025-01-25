//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PanLeft_H
#define PanLeft_H

#include "SndMan.h"

class PanLeft
{

public:
	// Big 4
	PanLeft() = default;
	PanLeft(const PanLeft&) = default;
	PanLeft& operator = (const PanLeft&) = default;
	~PanLeft() = default;

	void PanLeft_Main(Snd*& pSnd);
	void PanLeft_Stop(Snd*& pSnd);

public:
	// Data

};

#endif

// End of File 