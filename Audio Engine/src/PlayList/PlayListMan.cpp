//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PlaylistMan.h"
#include "Audio_LoadPlaylist_Cmd.h"
#include "QueueMan.h"

using namespace ThreadFramework;

PlaylistMan *PlaylistMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
PlaylistMan::PlaylistMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Playlist();
}

PlaylistMan::~PlaylistMan()
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
void PlaylistMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new PlaylistMan(reserveNum, reserveGrow);
    }

}

void PlaylistMan::Destroy()
{
    PlaylistMan *pMan = PlaylistMan::privGetInstance();
    assert(pMan != nullptr);

    delete PlaylistMan::posInstance;
    PlaylistMan::posInstance = nullptr;
}

Playlist *PlaylistMan::Add(Snd::ID snd_id, Wave::ID wave_id, VoiceCallback *pCallback)
{
    PlaylistMan *pMan = PlaylistMan::privGetInstance();

    Playlist *pNode = (Playlist *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the date
    pNode->Set(snd_id, wave_id, pCallback);

    return pNode;
}

Playlist *PlaylistMan::Find(Snd::ID _id)
{
    PlaylistMan *pMan = PlaylistMan::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->id = _id;

    Playlist *pData = (Playlist *) pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void PlaylistMan::Remove(Playlist *pNode)
{
    assert(pNode != nullptr);

    PlaylistMan *pMan = PlaylistMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void PlaylistMan::Dump()
{
    PlaylistMan *pMan = PlaylistMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
PlaylistMan *PlaylistMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *PlaylistMan::derivedCreateNode()
{
    DLink *pNodeBase = new Playlist();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

