//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef AUDIO_FILE_LOAD_COMPLETED_CMD_H
#define AUDIO_FILE_LOAD_COMPLETED_CMD_H

#include "Wave.h"
#include "Command.h"

struct Audio_FileLoadCompleted_Cmd : public Command
{
	Audio_FileLoadCompleted_Cmd() = delete;
	Audio_FileLoadCompleted_Cmd(const Audio_FileLoadCompleted_Cmd &) = delete;
	Audio_FileLoadCompleted_Cmd &operator = (const Audio_FileLoadCompleted_Cmd &) = delete;
	~Audio_FileLoadCompleted_Cmd() = default;

	Audio_FileLoadCompleted_Cmd(const char *const pWaveName, Wave *pWave);
	void LoadBuffer(const char *const pWaveName);

	void Execute() override;

	WAVEFORMATEXTENSIBLE	*poWfx;
	RawData					*poRawBuff;
	unsigned long           rawBuffSize;
	Wave                    *pWave;
};

#endif

//---  End of File ---

