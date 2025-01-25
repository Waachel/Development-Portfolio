//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STRING_THIS_H
#define STRING_THIS_H

#include "Handle.h"
#include "Voice.h"
#include "Wave.h"
#include "Snd.h"
#include "WaveTable.h"

class StringThis
{
public:
	static const unsigned int BUFFER_SIZE = 256;
public:
	StringThis(Handle::Status status);
	StringThis(Wave::ID id);
	StringThis(Snd::ID id);
	StringThis(Wave::Status id);

	operator char *();

	// data:
	char buffer[BUFFER_SIZE];
};

#define StringMe(x)  ((char *)StringThis(x)) 

#endif

// --- End of File ---
