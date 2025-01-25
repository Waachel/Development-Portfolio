//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "XAudio2Wrapper.h"
#include "AudioEngine.h"
#include "Playlist.h"
#include "StringThis.h"
#include "VoiceMan.h"
#include "Playlist_Start_Cmd.h"
#include "Playlist_Stop_Cmd.h"
#include "Playlist_Vol_Cmd.h"
#include "Playlist_Pan_Cmd.h"
#include "VoiceCallback_Stitched.h"
#include "QueueMan.h"
#include "PlaylistMan.h"

const float Playlist::Default_Pan = 0.0f;
const float Playlist::Default_Vol = 0.5f;

Playlist::Playlist()
	: pVoice(nullptr),
	vol(0.0f),
	pan(0.0f),
	poStartCmd(nullptr),
	poStopCmd(nullptr),
	poVolCmd(nullptr),
	poPanCmd(nullptr),
	id(Snd::ID::Uninitialized),
	type(Playlist::Type::Uninitialized)
{
	// nothing dynamic...

	// Pattern:
	//    Manager calls default wave
	//    set(...)  dynamic allocation
	//    privClear(...) release dynamic allocation
	//    destructor release of any owned objects

	this->SetVol(Playlist::Default_Vol);
	this->SetPan(Playlist::Default_Pan);
}

Playlist::~Playlist()
{
	//Debug::out("~Playlist()\n");
	if (this->pVoice)
	{
		VoiceMan::Remove(this->pVoice);
		this->pVoice = nullptr;
	}

	delete this->poStartCmd;
	delete this->poStopCmd;
	delete this->poVolCmd;
	delete this->poPanCmd;

	this->poStartCmd = nullptr;
	this->poStopCmd = nullptr;
	this->poVolCmd = nullptr;
	this->poPanCmd = nullptr;
}

void Playlist::SetId(Snd::ID _id)
{
	this->id = _id;
}

Snd::ID Playlist::GetId() const
{
	return this->id;
}

void Playlist::SetVol(float v)
{
	this->vol = v;
}

void Playlist::SetPan(float p)
{
	this->pan = p;
}

void Playlist::SndEnd()
{
	assert(this->id != Snd::ID::Uninitialized);
	assert(this->pVoice);

	// Stop the voice if its not already stopped
	this->pVoice->Stop();

	// Now remove it off the active
	PlaylistMan::Remove(this);
}

void Playlist::Set(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback)
{
	this->id = snd_id;
	this->type = Playlist::Type::Uninitialized;

	assert(pCallback);

	this->pVoice = VoiceMan::Add(wave_id, pCallback);
	assert(this->pVoice);

	pCallback->SetVoice(pVoice);
	pCallback->SetASnd(nullptr);

	this->poStartCmd = new Playlist_Start_Cmd(this->pVoice, this);
	this->poStopCmd = new Playlist_Stop_Cmd(this->pVoice, this);
	this->poVolCmd = new Playlist_Vol_Cmd(this->pVoice, this);
	this->poPanCmd = new Playlist_Pan_Cmd(this->pVoice, this);
}


void Playlist::Dump()
{
	// Dump - Print contents to the debug output window
	Trace::out("\t\tPlaylist(%p) %s Voice(%p) src:%p\n", this, StringMe(this->id), this->pVoice, this->pVoice->poSourceVoice);
}

void Playlist::privClear()
{
	// This method... is used in wash to reuse 
	// If its alive... remove the data
	if (this->pVoice)
	{
		VoiceMan::Remove(this->pVoice);
		this->pVoice = nullptr;
	}

	delete this->poStartCmd;
	delete this->poStopCmd;
	delete this->poVolCmd;
	delete this->poPanCmd;

	this->poStartCmd = nullptr;
	this->poStopCmd = nullptr;
	this->poVolCmd = nullptr;
	this->poPanCmd = nullptr;


	this->id = Snd::ID::Uninitialized;

}

void Playlist::Wash()
{
	// Wash - clear the entire hierarchy
	DLink::Clear();

	// Sub class clear
	this->privClear();
}

bool Playlist::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	Playlist *pDataB = (Playlist *)pTarget;

	bool status = false;

	if (this->id == pDataB->id)
	{
		status = true;
	}

	return status;
}





// --- End of File ---