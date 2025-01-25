//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef USER_SND_CALLBACK_H
#define USER_SND_CALLBACK_H

#include "Snd.h"
#include "ASnd.h"
#include "Wave.h"

class UserSndCallback
{
public:
	UserSndCallback() = default;
	UserSndCallback(const UserSndCallback &) = delete;
	UserSndCallback & operator = (const UserSndCallback &) = delete;
	~UserSndCallback() = default;

	void Set(ASnd *_pASnd, Snd *_pSnd, char *waveName);
	void Execute();

	Snd::ID GetSndID();

private:
	Snd		*pSnd;
	char	pWaveName[Wave::NAME_SIZE];
	Snd::ID snd_id;
	float   vol;
	float   pan;
	Time    timeStart;
	Time    timeEnd;

};

#endif

// --- End of File ---
