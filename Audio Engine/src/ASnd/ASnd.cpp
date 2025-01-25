//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ASnd.h"
#include "ASndMan.h"
#include "PlaylistJukeBox.h"
#include "PlaylistMan.h"
#include "StringThis.h"
#include "SndMan.h"
#include "UserSndCallBack.h"
#include "Game_TransferSndUserCallback_Cmd.h"
#include "QueueMan.h"

using namespace ThreadFramework;

ASnd::ASnd()
	:pPlaylist(nullptr),
	snd_id(Snd::ID::Uninitialized),
	pSnd(nullptr),
	pUserSndCallback(nullptr)
{
	// nothing dynamic...

	// Pattern:
	//    Manager calls default wave
	//    set(...)  dynamic allocation
	//    privClear(...) release dynamic allocation
	//    destructor release of any owned objects
}

ASnd::~ASnd()
{
	//Debug::out("~ASnd()\n");
}


void ASnd::SndEnd()
{
	assert(this->snd_id != Snd::ID::Uninitialized);
	assert(this->pPlaylist);

	//Debug::out("--------- Snd end() --------------\n");
	Debug::out("ASnd::SndEnd(%s)\n", StringMe(this->pSnd->snd_id));

	// UserCallback?
	if (this->pUserSndCallback)
	{
		assert(pPlaylist);
		assert(pPlaylist->pVoice);
		assert(pPlaylist->pVoice->pWave);
		char *pName = this->pPlaylist->pVoice->pWave->strName;
		assert(pName);

		this->pUserSndCallback->Set(this, this->pSnd, pName);

		//Debug::out("callback %p\n", this->pUserCallback);
		// Send a command to create ASnd
		Game_TransferSndUserCallback_Cmd *pCmd = new Game_TransferSndUserCallback_Cmd(this->pUserSndCallback);
		assert(pCmd);

		//Debug::out("--> Game_TransferUserCallback_Cmd \n");
		QueueMan::SendGame(pCmd);

	}

	// Playlist (which will deal with Voice)
	pPlaylist->SndEnd();

	// Snd needs to be alerted
	assert(this->pSnd);
	this->pSnd->RemoveFromPriorityTable();
	this->pSnd->pASnd = nullptr;
	SndMan::Remove(this->pSnd);

	// ASndMan - remove from the active list
	this->pSnd = nullptr;
	ASndMan::Remove(this);

}

void ASnd::Set(Snd::ID _snd_id, Snd *_pSnd)
{
	this->snd_id = _snd_id;

	assert(_pSnd);
	this->pSnd = _pSnd;

	pSnd->Set(this);

	// Find a reference
	Playlist *pPlayRef = PlaylistJukeBox::Find(this->snd_id);
	assert(pPlayRef);

	// Do a deep copy...
	// Problem: you don't know what type of callback is being used
	// Answer: ask the class to create one itself
	assert(pPlayRef->pVoice);
	assert(pPlayRef->pVoice->porCallback);

	VoiceCallback *pCallback;
	
	pCallback = pPlayRef->pVoice->porCallback->CreateCallbackCopy();
	assert(pCallback);

	this->pPlaylist = PlaylistMan::Add( _snd_id, 
										pPlayRef->pVoice->pWave->id,
										pCallback);

	// Make sure new playlist is TRULY independent
	assert(this->pPlaylist);
	pCallback->SetVoice(this->pPlaylist->pVoice);
	pCallback->SetASnd(this);

	assert(this->pPlaylist->pVoice);
	assert(this->pPlaylist->pVoice != pPlayRef->pVoice);
}

void ASnd::Play()
{
	assert(this->pPlaylist);
	assert(this->pPlaylist->poVolCmd);


	this->pPlaylist->poStartCmd->Execute();
	
	//Debug::out("ASnd::Play(%s)\n",StringMe(this->pSnd->snd_id));
}


void ASnd::Stop()
{
	assert(this->pPlaylist);
	assert(this->pPlaylist->poVolCmd);

	this->pPlaylist->poStopCmd->Execute();

	this->SndEnd();

	//Debug::out("ASnd::Stop(%p)\n",this);
}

void ASnd::Vol(float vol)
{
	assert(this->pPlaylist);
	assert(this->pPlaylist->poVolCmd);

	this->pPlaylist->SetVol(vol);
	this->pPlaylist->poVolCmd->Execute();

	//Debug::out("ASnd::Vol(%p)\n",this);
}

void ASnd::Pan(float pan)
{
	assert(this->pPlaylist);
	assert(this->pPlaylist->poVolCmd);

	this->pPlaylist->SetPan(pan);
	this->pPlaylist->poPanCmd->Execute();

	//Debug::out("ASnd::Vol(%p)\n",this);
}

void ASnd::Dump()
{
	// Dump - Print contents to the debug output window
	Trace::out("\t\tASnd(%p) %s Playlist(%p)\n", this, StringMe(this->snd_id), this->pPlaylist);
}

void ASnd::privClear()
{
	this->pPlaylist = nullptr;
	this->snd_id = Snd::ID::Uninitialized;

	// do not delete poUserCallback
	// Game will delete it
	//delete this->poUserCallback;
	//this->poUserCallback = nullptr;
}

void ASnd::Wash()
{
	// Wash - clear the entire hierarchy
	DLink::Clear();

	// Sub class clear
	this->privClear();
}

bool ASnd::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	ASnd *pDataB = (ASnd *) pTarget;

	bool status = false;

	if(this->snd_id == pDataB->snd_id)
	{
		status = true;
	}

	return status;
}





// --- End of File ---