//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SndMan.h"
#include "QueueMan.h"
#include "UserSndCallback.h"

using namespace ThreadFramework;

SndMan *SndMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
SndMan::SndMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Snd();
}

SndMan::~SndMan()
{
    // Debug::out("~Snd()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Snd *pDeleteMe = (Snd *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Snd *pDeleteMe = (Snd *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void SndMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new SndMan(reserveNum, reserveGrow);
    }
}

void SndMan::Destroy()
{
    SndMan *pMan = SndMan::privGetInstance();
    assert(pMan != nullptr);

    delete SndMan::posInstance;
    SndMan::posInstance = nullptr;
}

Snd *SndMan::Add(Snd::ID snd_id, UserSndCallback *pUserSndCallback)
{
    return SndMan::Add(snd_id, pUserSndCallback, SndMan::DEFAULT_PRIORITY);
}

Snd *SndMan::Add(Snd::ID snd_id, Snd::Priority priority)
{
    return SndMan::Add(snd_id, nullptr, priority);
}

Snd *SndMan::Add(Snd::ID snd_id)
{
    return SndMan::Add(snd_id, nullptr, SndMan::DEFAULT_PRIORITY);
}

Snd *SndMan::Add(Snd::ID snd_id, UserSndCallback *pUserSndCallback, Snd::Priority priority)
{
    SndMan *pMan = SndMan::privGetInstance();

    Snd *pNode = (Snd *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the date
    pNode->Set(snd_id, pUserSndCallback, priority);
    
    return pNode;
}

void SndMan::Update()
{
    Command *pCmd;
    CircularData *pGameIn = QueueMan::GetGameInQueue();

    if(pGameIn->PopFront(pCmd) == true)
    {
        assert(pCmd);
        pCmd->Execute();
    }
}

Snd *SndMan::Find(Snd::ID _id)
{
    SndMan *pMan = SndMan::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->snd_id = _id;

    Snd *pData = (Snd *) pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void SndMan::Remove(Snd *pNode)
{
    assert(pNode != nullptr);

    SndMan *pMan = SndMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void SndMan::Dump()
{
    SndMan *pMan = SndMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
SndMan *SndMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *SndMan::derivedCreateNode()
{
    DLink *pNodeBase = new Snd();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}



