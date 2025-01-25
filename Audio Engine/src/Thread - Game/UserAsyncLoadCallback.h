//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef USER_ASYNC_LOAD_CALLBACK_H
#define USER_ASYNC_LOAD_CALLBACK_H

#include "Snd.h"
#include "ASnd.h"
#include "Wave.h"
#include "TimerMan.h"

class UserAsyncLoadCallback
{
public:
	UserAsyncLoadCallback();
	UserAsyncLoadCallback(const UserAsyncLoadCallback&) = delete;
	UserAsyncLoadCallback& operator = (const UserAsyncLoadCallback&) = delete;
	~UserAsyncLoadCallback() = default;

	void Execute();

	void Set(Wave::ID id, const char* _pWaveName);
	Wave::ID GetWaveID();

private:
	// Return data...
	char	 pWaveName[Wave::NAME_SIZE];
	Wave::ID wave_id;
	Time    timeStart;
	Time    timeEnd;
};

#endif

// --- End of File ---
