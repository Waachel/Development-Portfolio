//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FILE_LOAD_FILE_COMMAND_H
#define FILE_LOAD_FILE_COMMAND_H

#include "Command.h"
#include "Wave.h"

class File_LoadFile_Cmd : public Command
{
public:
	// Big 4
	File_LoadFile_Cmd() = delete;
	File_LoadFile_Cmd(const File_LoadFile_Cmd &) = delete;
	File_LoadFile_Cmd &operator = (const File_LoadFile_Cmd &) = delete;
	~File_LoadFile_Cmd();

	File_LoadFile_Cmd(Wave::ID id, const char *const pWaveName, Wave *pWave);

	virtual void Execute() override;

public:
	// Data
	Wave::ID wave_id;
	const char *const pWaveName;
	Wave *pWave;
};

#endif

// --- End of File ---