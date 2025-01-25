//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_STOP_ASND_COMMAND_H
#define AUDIO_STOP_ASND_COMMAND_H

#include "Command.h"
#include "Snd.h"

class Audio_StopASnd_Cmd : public Command
{

public:
	// Big 4
	Audio_StopASnd_Cmd() = delete;
	Audio_StopASnd_Cmd(const Audio_StopASnd_Cmd&) = delete;
	Audio_StopASnd_Cmd& operator = (const Audio_StopASnd_Cmd&) = delete;
	~Audio_StopASnd_Cmd() = default;

	Audio_StopASnd_Cmd(Snd::ID snd_id, Snd* pSnd);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd* pSnd;
};

#endif

// --- End of File ---