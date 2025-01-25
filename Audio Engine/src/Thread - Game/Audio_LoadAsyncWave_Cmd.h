//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_LOAD_ASYNC_WAVE_COMMAND_H
#define AUDIO_LOAD_ASYNC_WAVE_COMMAND_H

#include "Command.h"
#include "Wave.h"
#include "UserAsyncLoadCallback.h"

class Audio_LoadAsyncWave_Cmd : public Command
{
public:
	// Big 4
	Audio_LoadAsyncWave_Cmd() = delete;
	Audio_LoadAsyncWave_Cmd(const Audio_LoadAsyncWave_Cmd&) = delete;
	Audio_LoadAsyncWave_Cmd& operator = (const Audio_LoadAsyncWave_Cmd&) = delete;
	~Audio_LoadAsyncWave_Cmd() = default;

	Audio_LoadAsyncWave_Cmd(Wave::ID id, const char* const pWaveName, UserAsyncLoadCallback* pUserAsyncLoadCallback);

	virtual void Execute() override;

public:
	// Data
	Wave::ID id;
	const char* const pWaveName;
	UserAsyncLoadCallback* pUserAsyncLoadCallback;
};

#endif

// --- End of File ---
