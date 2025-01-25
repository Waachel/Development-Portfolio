//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_REMOVE_WAVE_COMMAND_H
#define AUDIO_REMOVE_WAVE_COMMAND_H

#include "Command.h"
#include "Wave.h"
#include "I_File_CB.h"

class Audio_RemoveWave_Cmd : public Command
{
public:
	// Big 4
	Audio_RemoveWave_Cmd() = delete;
	Audio_RemoveWave_Cmd(const Audio_RemoveWave_Cmd&) = delete;
	Audio_RemoveWave_Cmd& operator = (const Audio_RemoveWave_Cmd&) = delete;
	~Audio_RemoveWave_Cmd() = default;

	Audio_RemoveWave_Cmd(Wave::ID id, I_File_CB* pFileCB);

	virtual void Execute() override;

public:
	// Data
	Wave::ID id;
	I_File_CB* pIFileCB;
};

#endif

// --- End of File ---
