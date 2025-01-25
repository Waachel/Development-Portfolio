//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_LOAD_WAVE_COMMAND_H
#define AUDIO_LOAD_WAVE_COMMAND_H

#include "Command.h"
#include "Wave.h"
#include "I_File_CB.h"

class Audio_LoadWave_Cmd : public Command
{
public:
	// Big 4
	Audio_LoadWave_Cmd() = delete;
	Audio_LoadWave_Cmd(const Audio_LoadWave_Cmd &) = delete;
	Audio_LoadWave_Cmd &operator = (const Audio_LoadWave_Cmd &) = delete;
	~Audio_LoadWave_Cmd() = default;

	Audio_LoadWave_Cmd(Wave::ID id, const char *const pWaveName, I_File_CB *pFileCB);

	virtual void Execute() override;

public:
	// Data
	Wave::ID id;
	const char *const pWaveName;
	I_File_CB *pIFileCB;
};

#endif

// --- End of File ---