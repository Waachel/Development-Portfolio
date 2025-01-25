//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "I_Playlist_CB.h"

using namespace ThreadFramework;

I_Playlist_CB::I_Playlist_CB(bool &DoneFlag)
	: rDoneFlag(DoneFlag)
{

}

void I_Playlist_CB::Execute()
{
	//Debug::out("I_Playlist_CB()::Execute()\n");

	this->rDoneFlag = true;

	delete this;
}

//---  End of File ---
