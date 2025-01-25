//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_REMOVE_ALL_WAVES_COMMAND_H
#define AUDIO_REMOVE_ALL_WAVES_COMMAND_H

#include "Command.h"
#include "Wave.h"
#include "I_File_CB.h"

class Audio_RemoveAllWaves_Cmd : public Command
{
public:
	// Big 4
	Audio_RemoveAllWaves_Cmd() = delete;
	Audio_RemoveAllWaves_Cmd(const Audio_RemoveAllWaves_Cmd&) = delete;
	Audio_RemoveAllWaves_Cmd& operator = (const Audio_RemoveAllWaves_Cmd&) = delete;
	~Audio_RemoveAllWaves_Cmd() = default;

	Audio_RemoveAllWaves_Cmd(I_File_CB* pFileCB);

	virtual void Execute() override;

public:
	// Data
	Wave::ID id;
	I_File_CB* pIFileCB;
};

#endif

// --- End of File ---
