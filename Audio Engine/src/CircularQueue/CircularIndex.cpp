//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "CircularIndex.h"

CircularIndex::CircularIndex(unsigned int buffSize)
{
	// Guarantee power of 2, and greater than 0
	assert((buffSize != 0) && ((buffSize & (buffSize - 1)) == 0));
	this->size = buffSize;
	this->mask = (unsigned int)(buffSize - 1);
	this->index = 0;
}

// postfix
unsigned int CircularIndex::operator++(int)
{
	this->index++;

	// Circular 
	this->index = this->index & this->mask;

	return this->index;
}

bool CircularIndex::operator==(const CircularIndex &tmp)
{
	assert(this->size == tmp.size);
	return (this->index == tmp.index);
}

bool CircularIndex::operator!=(const CircularIndex &tmp)
{
	assert(this->size == tmp.size);
	return (this->index != tmp.index);
}

unsigned int CircularIndex::getIndex() const
{
	return this->index;
}

//---  End of File ---
