//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_PLAY_ASND_COMMAND_H
#define AUDIO_PLAY_ASND_COMMAND_H

#include "Command.h"
#include "Snd.h"

class Audio_PlayASnd_Cmd : public Command
{

public:
	// Big 4
	Audio_PlayASnd_Cmd() = delete;
	Audio_PlayASnd_Cmd(const Audio_PlayASnd_Cmd &) = delete;
	Audio_PlayASnd_Cmd &operator = (const Audio_PlayASnd_Cmd &) = delete;
	~Audio_PlayASnd_Cmd() = default;

	Audio_PlayASnd_Cmd(Snd::ID snd_id, Snd *pSnd);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd *pSnd;
};

#endif

// --- End of File ---