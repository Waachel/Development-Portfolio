//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HANDLE_MAN_H
#define HANDLE_MAN_H

#include <shared_mutex>
#include "Handle.h"

class HandleMan
{
public:
	enum class Ready
	{
		UNINITIALIZED,
		INITIALIZED,
		DESTROYED
	};

private:

	// constants
	static const unsigned int TABLE_SIZE    = 256;
	static unsigned int INVALID_STATE;
	static unsigned int PENDING_STATE;
	static const unsigned int STARTING_ID = 0xAAAA0000;

	// Nested class
	class HandleTableEntry
	{
	public:
		HandleTableEntry() = default;
		HandleTableEntry(const HandleTableEntry &) = delete;
		HandleTableEntry &operator = (const HandleTableEntry &) = delete;
		~HandleTableEntry() = default;

	public:
		std::atomic<unsigned int> id;    
		std::shared_mutex		  mtx;     // <-- Reference count
	};


public:
	static void Create();
	static void Destroy();
	static Ready GetReadyStatus();

	//-----------------------------------------------------
	// You cannot copy the singleton
	//-----------------------------------------------------
	HandleMan(const HandleMan &) = delete;
	const HandleMan & operator = (const HandleMan &) = delete;

	//-----------------------------------------------------
	// Being strategic on what is expose or not
	//-----------------------------------------------------

	// Hold to use
	static Handle::Status IsValidAcquire(const Handle &handle);
	static Handle::Status IsValidRelease(const Handle &handle);

	// Nuke and Reset
	static Handle::Status InvalidateHandle(const Handle &handle);
	static Handle::Status ActivateHandle(Handle::ID &id, Handle::Index &index);

	//-----------------------------------------------------
	// Only for our DEBUG - remove for application
	//-----------------------------------------------------
	static void PrintTable();

private:
	HandleMan();
	~HandleMan();

	//-----------------------------------------------------
	// Keeping internals... private
	//-----------------------------------------------------
	static HandleMan *privInstance();
	static HandleMan *psInstance;
	static HandleMan::Ready ready;

	Handle::ID privGetNewID();
	bool privFindNextAvailable(Handle::Index &index);
	Handle::Status privIsHandleAndRangeValid(const Handle &handle);

	//-----------------------------------------------------
	// Raw data
	//-----------------------------------------------------
	HandleTableEntry     table[TABLE_SIZE];  
	Handle::ID           srcID;

};

#endif

// ---  End of File ---------------
