//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HandleMan.h"

using namespace ThreadFramework;

HandleMan *HandleMan::psInstance = nullptr;
HandleMan::Ready HandleMan::ready = HandleMan::Ready::UNINITIALIZED;

unsigned int HandleMan::INVALID_STATE = 0;
unsigned int HandleMan::PENDING_STATE = 0xFFFFFFFF;

// --------------------------------------------------------------------
//   No Mutex protection
// --------------------------------------------------------------------

void HandleMan::Create()
{
	assert(HandleMan::psInstance == nullptr);
	HandleMan::psInstance = new HandleMan();
	assert(HandleMan::psInstance);
	HandleMan::ready = HandleMan::Ready::INITIALIZED;
}

void HandleMan::Destroy()
{
	Trace::out("HandleMan::Destroy()\n");
	assert(HandleMan::psInstance);

	delete HandleMan::psInstance;
	HandleMan::psInstance = nullptr;

	HandleMan::ready = HandleMan::Ready::DESTROYED;
}

HandleMan::HandleMan()
{
	this->srcID = STARTING_ID;

	for(int i = 0; i < TABLE_SIZE; i++)
	{
		this->table[i].id.store(INVALID_STATE);
	}
}

HandleMan::Ready HandleMan::GetReadyStatus()
{
	return HandleMan::ready;
}

HandleMan::~HandleMan()
{
	// safety - nuke it again
	this->srcID = STARTING_ID;

	for(int i = 0; i < TABLE_SIZE; i++)
	{
		this->table[i].id.store(INVALID_STATE);
	}
}

Handle::ID HandleMan::privGetNewID()
{
	// Increment
	this->srcID++;

	return this->srcID;
}

// --------------------------------------------------------------------
//   No Mutex protection
// --------------------------------------------------------------------

Handle::Status HandleMan::InvalidateHandle(const Handle &handle)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	// Is Range and ID valid?
	Handle::Status status = pMan->privIsHandleAndRangeValid(handle);

	if(status == Handle::Status::VALID_HANDLE)
	{
		// Block until you get the exclusive lock
		pMan->table[handle.GetIndex()].mtx.lock();

		// ---------------------------------------------------------------------
		// Race condition:
		// What happens if the ID is invalidated between the test and the lock?
		// ---------------------------------------------------------------------

			// Is ID valid?
			status = pMan->privIsHandleAndRangeValid(handle);

			if(status == Handle::Status::VALID_HANDLE)
			{
				// nuke it
				pMan->table[handle.GetIndex()].id.store(INVALID_STATE);

				// release the mtx
				pMan->table[handle.GetIndex()].mtx.unlock();

				status = Handle::Status::INVALID_HANDLE;
			}
		else if (status == Handle::Status::INVALID_HANDLE)
			{
				// race condition... handle is invalid - status is ERROR

				// release the mtx
				pMan->table[handle.GetIndex()].mtx.unlock();
			}
		else
		{
			// Something bad happened
			assert(false);
			status = Handle::Status::HANDLE_ERROR;
		}
	}
	else if (status == Handle::Status::INVALID_HANDLE)
	{
		// its already invalidated
		}
		else
		{
		// Something bad happened
		assert(false);
			status = Handle::Status::HANDLE_ERROR;
		}

	return status;
}

Handle::Status HandleMan::IsValidAcquire(const Handle &handle)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	// is it valid?
	Handle::Status status = pMan->privIsHandleAndRangeValid(handle);

	if (status == Handle::Status::VALID_HANDLE)
	{
		// get shared mutex
		pMan->table[handle.GetIndex()].mtx.lock_shared();

		// Double check... in case of Race condition
		status = pMan->privIsHandleAndRangeValid(handle);

		if (status == Handle::Status::VALID_HANDLE)
		{
			// all good!
	}
	else
	{
			// not valid... 
			// so unlock it
			pMan->table[handle.GetIndex()].mtx.unlock_shared();
		}

	}

	return status;
}

Handle::Status HandleMan::IsValidRelease(const Handle &handle)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	// is it valid?
	Handle::Status status = pMan->privIsHandleAndRangeValid(handle);

	if (status == Handle::Status::VALID_HANDLE)
	{
		// get shared mutex
		pMan->table[handle.GetIndex()].mtx.unlock_shared();
	}
	else
	{
		// Something bad has happened...
		assert(false);
	}

	return status;
}

Handle::Status HandleMan::ActivateHandle(Handle::ID &new_id, Handle::Index &index)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	Handle::Status 	status = Handle::Status::HANDLE_ERROR;

	if(pMan->privFindNextAvailable(index))
	{
		// Block until you get the exclusive lock
		pMan->table[index].mtx.lock();

		// ---------------------------------------------------------------------
		// Race condition:
		// What happens if the ID is invalidated between the test and the lock?
		// ---------------------------------------------------------------------
		if (pMan->table[index].id.load() != PENDING_STATE)
		{
				// unlock
				pMan->table[index].mtx.unlock();

				// bail and fall through
			status = Handle::Status::HANDLE_ERROR;

			assert(false);
			}
		else
		{
			// Set it - atomic!
			new_id = pMan->privGetNewID();
			pMan->table[index].id.store(new_id);

			// unlock
			pMan->table[index].mtx.unlock();

			status = Handle::Status::VALID_HANDLE;
		}
	}
	else
	{
		status = Handle::Status::INSUFFIENT_SPACE;

		// trap for Debug
		assert(status == Handle::Status::SUCCESS);
	}

	return status;
}

// --------------------------------------------------------------------
//   No Mutex protection
// --------------------------------------------------------------------

bool HandleMan::privFindNextAvailable(Handle::Index &index)
{
	bool status = false;

	for(Handle::Index i = 0; i < TABLE_SIZE; i++)
	{
		if(	this->table[i].id.compare_exchange_strong(INVALID_STATE,PENDING_STATE)   )
		{
			// found one
			index = i;
			status = true;
			break;
		}
	}

	return status;
}


Handle::Status HandleMan::privIsHandleAndRangeValid(const Handle &handle)
{
	Handle::Status status = Handle::Status::HANDLE_ERROR;

	if( handle.GetIndex() < TABLE_SIZE)
	{
		if(this->table[handle.GetIndex()].id.load() == handle.GetID())
		{
			status = Handle::Status::VALID_HANDLE;
		}
		else
		{
			status = Handle::Status::INVALID_HANDLE;
		}
	}
	else
	{
		status = Handle::Status::HANDLE_ERROR;
	}

	return status;
}

HandleMan *HandleMan::privInstance()
{
	HandleMan *pInstance = nullptr;

	// Here's the singleton
	if(HandleMan::psInstance)
	{
		pInstance = HandleMan::psInstance;
	}
	else
	{
		assert(false);
	}

	return pInstance;
}

void HandleMan::PrintTable()
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	Trace::out("\n");

	// No protection.... Just printing
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		if(pMan->table[i].id == INVALID_STATE)
		{
			Trace::out("[%d]: %s \n", i, STRING_ME(INVALID_STATE));
		}
		else
		{
			Trace::out("[%d]: %x \n", i, pMan->table[i].id.load());
		}
	}

	Trace::out("\n");
}

// ---  End of File ---------------
