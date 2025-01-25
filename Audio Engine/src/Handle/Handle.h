//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HANDLE_H
#define HANDLE_H

//---------------------------------------------------------------
// Handle class
// --------------------------------------------------------------

class Handle
{
public:

	// --------------------------------------------------
	// Status of handles
	// --------------------------------------------------
	enum class Status
	{
		SUCCESS = 0x7EEE0000,

		INSUFFIENT_SPACE,
		INVALID_HANDLE,
		VALID_HANDLE,

		HANDLE_ERROR = 0x7EEEFFFF
	};

	// --------------------------------------------------
	// Handle Lock paradigm
	// --------------------------------------------------

	class Lock
	{
	public:
		enum class Status
		{
			LOCKED,
			UNLOCKED
		};

	public:
		Lock(const Handle &h);

		Lock() = delete;
		Lock(const Lock &) = delete;
		Lock &operator = (const Lock &) = delete;
		~Lock();

		// boolean operator
		operator bool() const;

		// manually unlock
		void unlock();
		void lock();

		// conversion operator
		operator Handle::Status() const;

	private:
		const Handle &handle;   // no copy just a pointer(ref) to original
		Lock::Status status;
	};
	class LockTwoInput
	{
	public:
		enum class Status
		{
			LOCKED,
			UNLOCKED
		};

	public:
		LockTwoInput(Handle &this_handle, const Handle &input_handle);

		LockTwoInput() = delete;
		LockTwoInput(const LockTwoInput &) = delete;
		LockTwoInput &operator = (const LockTwoInput &) = delete;
		~LockTwoInput();

		// boolean operator
		operator bool() const;

		// manually unlock
		void unlock();
		void lock();

		// conversion operator
		operator Handle::Status() const;

	private:
		Handle &this_handle;   // no copy just a pointer(ref) to original
		const Handle &input_handle;   // no copy just a pointer(ref) to original
		LockTwoInput::Status status;
	};

	#define CopyConstructorLock LockTwoInput
	#define AssignOperatorLock  LockTwoInput

	// --------------------------------------------------
	// Specialized types
	// --------------------------------------------------
	typedef unsigned int  ID;
	typedef unsigned int  Index;

public:
	Handle();
	~Handle();

	//-----------------------------------------------------
	// Do not copy the handle... 
	//-----------------------------------------------------
	Handle(const Handle &) = delete;
	Handle & operator = (const Handle &) = delete;

	//-----------------------------------------------------
	// You can peek at it, but you can't set it 
	//-----------------------------------------------------
	ID GetID() const;
	Index GetIndex() const;

	//-----------------------------------------------------
	// Tunnel method to prevent dependency on HandleMan
	//-----------------------------------------------------
	
        // Hold to use
	static Status AcquireHandle(const Handle &handle);
	static Status ReleaseHandle(const Handle &handle);

	// Nuke and Reset
	static Status InvalidateHandle(const Handle &handle);
	static Status ActivateHandle(Handle &handle);

private:
	//-----------------------------------------------------
	// Data must remain private
	//-----------------------------------------------------
	ID		id;
	Index	index;
};

#endif

// ---  End of File ---------------
