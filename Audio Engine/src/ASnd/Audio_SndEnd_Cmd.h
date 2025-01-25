//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_SND_END_COMMAND_H
#define AUDIO_SND_END_COMMAND_H

#include "Command.h"
#include "Snd.h"

class Audio_SndEnd_Cmd : public Command
{

public:
	// Big 4
	Audio_SndEnd_Cmd() = delete;
	Audio_SndEnd_Cmd(const Audio_SndEnd_Cmd &) = delete;
	Audio_SndEnd_Cmd &operator = (const Audio_SndEnd_Cmd &) = delete;
	~Audio_SndEnd_Cmd() = default;

	Audio_SndEnd_Cmd(Snd::ID snd_id, Snd *pSnd);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd *pSnd;
};

#endif

// --- End of File ---
