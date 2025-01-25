//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef AUDIO_ASYNC_FILE_LOAD_COMPLETED_CMD_H
#define AUDIO_ASYNC_FILE_LOAD_COMPLETED_CMD_H

#include "Wave.h"
#include "Command.h"

struct Audio_AsyncFileLoadCompleted_Cmd : public Command
{
	Audio_AsyncFileLoadCompleted_Cmd() = delete;
	Audio_AsyncFileLoadCompleted_Cmd(const Audio_AsyncFileLoadCompleted_Cmd&) = delete;
	Audio_AsyncFileLoadCompleted_Cmd& operator = (const Audio_AsyncFileLoadCompleted_Cmd&) = delete;
	~Audio_AsyncFileLoadCompleted_Cmd() = default;

	Audio_AsyncFileLoadCompleted_Cmd(const char* const pWaveName, Wave* pWave);
	void LoadBuffer(const char* const pWaveName);

	void Execute() override;

	WAVEFORMATEXTENSIBLE* poWfx;
	RawData* poRawBuff;
	unsigned long           rawBuffSize;
	Wave* pWave;
};

#endif

//---  End of File ---

