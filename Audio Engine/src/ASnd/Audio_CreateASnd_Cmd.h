//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AUDIO_CREATE_ASND_COMMAND_H
#define AUDIO_CREATE_ASND_COMMAND_H

#include "Command.h"
#include "Snd.h"
#include "UserSndCallBack.h"

class Audio_CreateASnd_Cmd : public Command
{

public:
	// Big 4
	Audio_CreateASnd_Cmd() = delete;
	Audio_CreateASnd_Cmd(const Audio_CreateASnd_Cmd &) = delete;
	Audio_CreateASnd_Cmd &operator = (const Audio_CreateASnd_Cmd &) = delete;
	~Audio_CreateASnd_Cmd() = default;

	Audio_CreateASnd_Cmd(Snd::ID snd_id, Snd *pSnd, UserSndCallback *pUserCallback);

	virtual void Execute() override;

public:
	// Data
	Snd::ID snd_id;
	Snd    *pSnd;
	UserSndCallback *pUserSndCallback;
};

#endif

// --- End of File ---