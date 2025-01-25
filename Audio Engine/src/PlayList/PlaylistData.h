//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PLAYLIST_DATA_H
#define PLAYLIST_DATA_H

#include "XAudio2Wrapper.h"

// p - pointer
// po - pointer owned (composition)
// ps - pointer to static
// por - pointer owned recieved
// pc - pointer created then transferred to por

class PlaylistData
{
public:
	PlaylistData() = default;
	PlaylistData(const PlaylistData &) = delete;
	PlaylistData &operator = (const PlaylistData &) = delete;
	virtual ~PlaylistData() = default;

	virtual Wave::ID GetFirstWaveID() = 0;
	virtual VoiceCallback *Create_VoiceCallback() = 0;

};

#endif

// --- End of File ---