//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "VoiceMan.h"
#include "WaveMan.h"

using namespace ThreadFramework;

VoiceMan *VoiceMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
VoiceMan::VoiceMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Voice();
}

VoiceMan::~VoiceMan()
{
    //Debug::out("~VoiceMan()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Voice *pDeleteMe = (Voice *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Voice *pDeleteMe = (Voice *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void VoiceMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new VoiceMan(reserveNum, reserveGrow);
    }

}

void VoiceMan::Destroy()
{
    VoiceMan *pMan = VoiceMan::privGetInstance();
    assert(pMan != nullptr);

    delete VoiceMan::posInstance;
    VoiceMan::posInstance = nullptr;
}

Voice *VoiceMan::Add(Wave::ID wave_id, VoiceCallback *pCallback)
{
    VoiceMan *pMan = VoiceMan::privGetInstance();

    // Find the wave from the manager
    Wave *pWave = WaveMan::Find(wave_id);
    assert(pWave);

    Voice *pNode = (Voice *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the date
    pNode->Set(pWave, pCallback);
    return pNode;
}


void VoiceMan::Remove(Voice *pNode)
{
    assert(pNode != nullptr);

    VoiceMan *pMan = VoiceMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void VoiceMan::Dump()
{
    VoiceMan *pMan = VoiceMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
VoiceMan *VoiceMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *VoiceMan::derivedCreateNode()
{
    DLink *pNodeBase = new Voice();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

