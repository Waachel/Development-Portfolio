//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AlarmCmd_H
#define AlarmCmd_H

#include "Command.h"
#include "AzulTime.h"

class AlarmCmd : public Command
{
public:
	AlarmCmd() = default;
	AlarmCmd(const AlarmCmd&) = default;
	AlarmCmd& operator = (const AlarmCmd&) = default;
	virtual ~AlarmCmd() = default;

	virtual void Execute() override;

public:
	// Data

};

#endif

// --- End of File ---
