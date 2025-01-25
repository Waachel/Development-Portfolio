//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef I_PLAYLIST_CB_H
#define I_PLAYLIST_CB_H

class I_Playlist_CB
{
public:
	I_Playlist_CB(bool &DoneFlag);
	I_Playlist_CB(const I_Playlist_CB &) = delete;
	I_Playlist_CB &operator = (const I_Playlist_CB &) = delete;
	~I_Playlist_CB() = default;

	void Execute();

private:
	bool &rDoneFlag;
};

#endif

//---  End of File ---
