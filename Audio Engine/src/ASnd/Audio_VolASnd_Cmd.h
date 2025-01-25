//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_VOL_ASND_COMMAND_H
#define AUDIO_VOL_ASND_COMMAND_H

#include "Command.h"
#include "Snd.h"

class Audio_VolASnd_Cmd : public Command
{

public:
	// Big 4
	Audio_VolASnd_Cmd() = delete;
	Audio_VolASnd_Cmd(const Audio_VolASnd_Cmd&) = delete;
	Audio_VolASnd_Cmd& operator = (const Audio_VolASnd_Cmd&) = delete;
	~Audio_VolASnd_Cmd() = default;

	Audio_VolASnd_Cmd(Snd::ID snd_id, Snd *pSnd, float _vol);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd* pSnd;
	float vol;
};

#endif

// --- End of File ---