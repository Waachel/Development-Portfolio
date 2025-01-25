//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PLAYLIST_DATA_ONE_H
#define PLAYLIST_DATA_ONE_H

#include "XAudio2Wrapper.h"
#include "PlaylistData.h"
#include "Wave.h"
#include "VoiceCallback_One.h"

// p - pointer
// po - pointer owned (composition)
// ps - pointer to static
// por - pointer owned recieved
// pc - pointer created then transferred to por

class PlaylistData_One : public PlaylistData
{
public:
	PlaylistData_One() = delete;
	PlaylistData_One(const PlaylistData_One &) = delete;
	PlaylistData_One &operator = (const PlaylistData_One &) = delete;
	virtual ~PlaylistData_One() = default;

	PlaylistData_One(Wave::ID wave)
		: PlaylistData(),wave_id(wave)
	{
	}

	virtual Wave::ID GetFirstWaveID() override
	{
		return this->wave_id;
	}

	virtual VoiceCallback *Create_VoiceCallback() override
	{
		return new VoiceCallback_One();
	}

	Wave::ID wave_id;

};

#endif

// --- End of File ---