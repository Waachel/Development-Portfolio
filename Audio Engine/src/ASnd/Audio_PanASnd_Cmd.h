//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_PAN_ASND_COMMAND_H
#define AUDIO_PAN_ASND_COMMAND_H

#include "Command.h"
#include "Snd.h"

class Audio_PanASnd_Cmd : public Command
{

public:
	// Big 4
	Audio_PanASnd_Cmd() = delete;
	Audio_PanASnd_Cmd(const Audio_PanASnd_Cmd&) = delete;
	Audio_PanASnd_Cmd& operator = (const Audio_PanASnd_Cmd&) = delete;
	~Audio_PanASnd_Cmd() = default;

	Audio_PanASnd_Cmd(Snd::ID snd_id, Snd *pSnd, float _vol);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd* pSnd;
	float pan;
};

#endif

// --- End of File ---