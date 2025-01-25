//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HandleMan.h"
#include "Handle.h"

using namespace ThreadFramework;

// --------------------------------------------------
// Handle::Lock paradigm
// --------------------------------------------------

//----------------------------------------------------------------------------
// Observation by Andrew Trumbo - why not a HandleLock?
//    You are correct sir!  Great idea
// Further Jedi Points to Trumbo ...
//    Conversion operator
//    bool operator
//----------------------------------------------------------------------------

Handle::LockTwoInput::LockTwoInput(Handle &h_this, const Handle &h_input)
	: this_handle(h_this),
	input_handle(h_input),
	status(LockTwoInput::Status::UNLOCKED)
{
	this->lock();
}

Handle::LockTwoInput::~LockTwoInput()
{
	// release mutexes;
	this->unlock();
}

// boolean operator
Handle::LockTwoInput::operator bool() const
{
	return (this->status == LockTwoInput::Status::LOCKED);
}

void Handle::LockTwoInput::unlock()
{
	if (this->status == LockTwoInput::Status::LOCKED)
	{
		const Handle::Status h_status = Handle::ReleaseHandle(this_handle);
		const Handle::Status h2_status = Handle::ReleaseHandle(input_handle);
		if (h_status == Handle::Status::VALID_HANDLE && h2_status == Handle::Status::VALID_HANDLE)
		{
			this->status = LockTwoInput::Status::UNLOCKED;
		}
		else
		{
			// make sure both handles are locked and valid
			assert(false);
		}
	}
	else
	{
		// already unlocked
		// do nothing
	}
}

void Handle::LockTwoInput::lock()
{
	if(this->status == LockTwoInput::Status::UNLOCKED)
	{
		Handle::Status h_status = Handle::AcquireHandle(this_handle);
		if(h_status == Handle::Status::VALID_HANDLE )
		{
			Handle::Status h2_status = Handle::AcquireHandle(input_handle);
			if(h2_status == Handle::Status::VALID_HANDLE)
			{
				this->status = LockTwoInput::Status::LOCKED;
			}
			else
			{
				Handle::Status s = Handle::ReleaseHandle(this_handle);
				assert(s == Handle::Status::VALID_HANDLE);
				s = Handle::InvalidateHandle(this_handle);
				assert(s == Handle::Status::INVALID_HANDLE);
				this->status = LockTwoInput::Status::UNLOCKED;
			}
		}
		else
		{
			if(h_status == Handle::Status::INVALID_HANDLE)
			{
				// do nothing... its already invalid
					//Handle::Status s = Handle::InvalidateHandle(this_handle);
					//assert(s == Handle::Status::INVALID_HANDLE);
					//this->status = LockTwoInput::Status::UNLOCKED;
			}
			else
			{   // case: Handle::Status::HANDLE_ERROR
					// do nothing... its still not valid
			}
		}
	}
	else
	{
		// already locked
	}
}


// conversion operator
Handle::LockTwoInput::operator Handle::Status() const
{
	Handle::Status h_status;

	if(this->status == LockTwoInput::Status::LOCKED)
	{
		h_status = Handle::Status::SUCCESS;
	}
	else
	{
		h_status = Handle::Status::HANDLE_ERROR;
	}
	return h_status;
}