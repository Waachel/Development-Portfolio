//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FILE_ASYNC_LOAD_FILE_COMMAND_H
#define FILE_ASYNC_LOAD_FILE_COMMAND_H

#include "Command.h"
#include "Wave.h"

class File_AsyncLoadFile_Cmd : public Command
{
public:
	// Big 4
	File_AsyncLoadFile_Cmd() = delete;
	File_AsyncLoadFile_Cmd(const File_AsyncLoadFile_Cmd&) = delete;
	File_AsyncLoadFile_Cmd& operator = (const File_AsyncLoadFile_Cmd&) = delete;
	~File_AsyncLoadFile_Cmd();

	File_AsyncLoadFile_Cmd(Wave::ID id, const char* const pWaveName, Wave* pWave);

	virtual void Execute() override;

public:
	// Data
	Wave::ID wave_id;
	const char* const pWaveName;
	Wave* pWave;
};

#endif

// --- End of File ---
