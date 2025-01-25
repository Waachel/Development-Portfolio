//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PlaylistJukeBox.h"
#include "Audio_LoadPlaylist_Cmd.h"
#include "QueueMan.h"
#include "I_Playlist_CB.h"

using namespace ThreadFramework;

PlaylistJukeBox *PlaylistJukeBox::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
PlaylistJukeBox::PlaylistJukeBox(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Playlist();
}

PlaylistJukeBox::~PlaylistJukeBox()
{
    // Debug::out("~Playlist()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Playlist *pDeleteMe = (Playlist *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Playlist *pDeleteMe = (Playlist *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void PlaylistJukeBox::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new PlaylistJukeBox(reserveNum, reserveGrow);
    }

}

void PlaylistJukeBox::Destroy()
{
    PlaylistJukeBox *pMan = PlaylistJukeBox::privGetInstance();
    assert(pMan != nullptr);

    delete PlaylistJukeBox::posInstance;
    PlaylistJukeBox::posInstance = nullptr;
}

void PlaylistJukeBox::Load(Snd::ID snd_id, PlaylistData *pPlaylistData)
{
    assert(pPlaylistData);

    // Setup the Callback
    bool DoneFlag = false;
    I_Playlist_CB *pPlaylistCB = new I_Playlist_CB(DoneFlag);
    assert(pPlaylistCB);

    Audio_LoadPlaylist_Cmd *pCmd = new Audio_LoadPlaylist_Cmd(snd_id, pPlaylistData, pPlaylistCB);
    assert(pCmd);

 //   Debug::out("--> Audio_LoadPlaylist_Cmd \n");
    QueueMan::SendAudio(pCmd);

    // Block and spin until Callback
    while (!DoneFlag);
}

Playlist *PlaylistJukeBox::Add(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback)
{
    PlaylistJukeBox *pMan = PlaylistJukeBox::privGetInstance();

    Playlist *pNode = (Playlist *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the date
    assert(pCallback);
    pNode->Set(snd_id, wave_id, pCallback);

    return pNode;
}

Playlist *PlaylistJukeBox::Find(Snd::ID _id)
{
    PlaylistJukeBox *pMan = PlaylistJukeBox::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->id = _id;

    Playlist *pData = (Playlist *) pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void PlaylistJukeBox::Remove(Playlist *pNode)
{
    assert(pNode != nullptr);

    PlaylistJukeBox *pMan = PlaylistJukeBox::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void PlaylistJukeBox::Dump()
{
    PlaylistJukeBox *pMan = PlaylistJukeBox::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
PlaylistJukeBox *PlaylistJukeBox::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *PlaylistJukeBox::derivedCreateNode()
{
    DLink *pNodeBase = new Playlist();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

