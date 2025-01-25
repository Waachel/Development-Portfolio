//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HandleMan.h"
#include "Handle.h"

using namespace ThreadFramework;

// --------------------------------------------------
// Handle
// --------------------------------------------------

Handle::Handle()
{
	//Activate
	Status status = Handle::ActivateHandle(*this);
	assert(status == Handle::Status::VALID_HANDLE);

	// Trace::out("Handle(): new handle(%x):0x%X\n", this->index, this->id);
}

Handle::~Handle()
{
	// Invalidate handle;
	if (HandleMan::Ready::INITIALIZED == HandleMan::GetReadyStatus())
	{
	HandleMan::InvalidateHandle(*this);

		// Trace::out("~Handle(): invalidate handle(%x):0x%X\n", this->index, this->id);
	}
	else
	{
		// Does this ever happen
		assert(false);
	}
}

Handle::ID Handle::GetID() const
{
	return this->id;
}

Handle::Index Handle::GetIndex() const
{
	return this->index;
}


//---------------------------------------------------------------
// Tunneling... I only want handle.h to be used
//---------------------------------------------------------------

Handle::Status Handle::AcquireHandle(const Handle &handle)
{
	return HandleMan::IsValidAcquire(handle);
}

Handle::Status Handle::ReleaseHandle(const Handle &handle)
	{
	return HandleMan::IsValidRelease(handle);
	}

	Handle::Status Handle::InvalidateHandle(const Handle &handle)
	{
		return HandleMan::InvalidateHandle(handle);
	}

Handle::Status Handle::ActivateHandle(Handle &handle)
{
	//Activate
	Status status = HandleMan::ActivateHandle(handle.id, handle.index);
	assert(status == Handle::Status::VALID_HANDLE);

	// Trace::out("activate id: %x \n", handle.id);
	return status;
}

// ---  End of File ---
