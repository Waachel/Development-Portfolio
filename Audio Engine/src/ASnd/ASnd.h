//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ASND_H
#define ASND_H

#include "XAudio2Wrapper.h"
#include "Handle.h"
#include "DLink.h"
#include "Snd.h"
#include "Voice.h"
#include "Playlist.h"

class UserSndCallback;

class ASnd : public DLink
{
public:
	// Big 4
	ASnd();
	ASnd(const ASnd &) = delete;
	ASnd &operator = (const ASnd &) = delete;
	virtual ~ASnd();

	void Set(Snd::ID snd_id, Snd *pSnd);

	void Dump();
	void Wash();

	virtual bool Compare(DLink *pTargetNode) override;

	// ---- Commands ---
	void Play();
	void Stop();
	void Vol(float vol);
	void Pan(float pan);

	// Stop and remove all
	void SndEnd();

private:
	void privClear();


public:
	//----------------------------------------------------
	// Data
	//----------------------------------------------------

	Playlist *pPlaylist;
	Snd::ID snd_id;
	Snd *pSnd;
	UserSndCallback *pUserSndCallback;

	Handle handle;
};

#endif

// --- End of File ---