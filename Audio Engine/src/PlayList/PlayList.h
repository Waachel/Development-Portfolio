//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Playlist_H
#define Playlist_H

#include "XAudio2Wrapper.h"
#include "Handle.h"
#include "DLink.h"
#include "Snd.h"
#include "Voice.h"
#include "PlaylistCmd.h"
#include "VoiceCallback.h"

class Playlist : public DLink
{
public:
	// Stupid compiler... can't initialize inside
	static const float Default_Pan;
	static const float Default_Vol;

public:
	enum class Type
	{
		PlayOnce,
		PlayRepeated,
		Stitched,
		Default = PlayOnce,
		Uninitialized
	};

public:
	// Big 4
	Playlist();
	Playlist(const Playlist &) = delete;
	Playlist &operator = (const Playlist &) = delete;
	virtual ~Playlist();

	void Set(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback);

	void SetId(Snd::ID id);
	Snd::ID GetId() const;

	void SetVol(float v);
	void SetPan(float p);
	
	void SndEnd();

	void Dump();
	void Wash();

	virtual bool Compare(DLink *pTargetNode) override;

private:
	void privClear();
	void privLoadBuffer(const char *const pWaveName);
	void privSetName(const char *const pWaveName);

public:
	//----------------------------------------------------
	// Data
	//----------------------------------------------------

	// In the future it can be many...
	Voice *pVoice;

	float vol;
	float pan;

	// Command pattern
	PlaylistCmd *poStartCmd;
	PlaylistCmd *poStopCmd;
	PlaylistCmd *poVolCmd;
	PlaylistCmd *poPanCmd;

	Snd::ID id;
	Type    type;

	Handle handle;
};

#endif

// --- End of File ---