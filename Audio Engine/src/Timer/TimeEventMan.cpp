//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TimeEventMan.h"

using namespace ThreadFramework;

TimeEventMan* TimeEventMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TimeEventMan::TimeEventMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new TimeEvent();

    this->GameTime.tic();
    this->mCurrTime = this->GameTime.toc();
}

TimeEventMan::~TimeEventMan()
{
    // Debug::out("~WaveMan()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator* pIt = this->baseGetActiveIterator();

    DLink* pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        TimeEvent* pDeleteMe = (TimeEvent*)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        TimeEvent* pDeleteMe = (TimeEvent*)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void TimeEventMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new TimeEventMan(reserveNum, reserveGrow);
    }

}

void TimeEventMan::Destroy()
{
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    delete TimeEventMan::posInstance;
    TimeEventMan::posInstance = nullptr;
}

void TimeEventMan::Reset()
{
    // Get the instance
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan);

    pMan->GameTime.tic();
    pMan->mCurrTime = pMan->GameTime.toc();
}

void TimeEventMan::UpdateTimeOnly()
{
    // Get the instance
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan);

    pMan->mCurrTime = pMan->GameTime.toc();
}

void TimeEventMan::Update()
{
    // Get the instance
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan);

    // squirrel away
    pMan->mCurrTime = pMan->GameTime.toc();

    // iterate through the list and delete
    Iterator* pIt = pMan->baseGetActiveIterator();

    // Walk through the nodes
    DLink* pNode = pIt->First();

    while (!pIt->IsDone())
    {
        TimeEvent* pTimeEvent = (TimeEvent*)pNode;
        pNode = pIt->Next();

        if (pMan->mCurrTime >= (pTimeEvent->GetTriggerTime()))
        {
            // call it
            pTimeEvent->Process();

            // remove from list
            pMan->baseRemove(pTimeEvent);
        }

    }
}


TimeEvent* TimeEventMan::Add(Command* pCommand, Time deltaTimeToTrigger)
{
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    TimeEvent* pNode = (TimeEvent*)pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Create a new one given a wash
    assert(pCommand);
    pNode->Set(pCommand, deltaTimeToTrigger);

    return pNode;
}

TimeEvent* TimeEventMan::Find(TimeEvent::ID _id)
{
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->SetID(_id);

    TimeEvent* pData = (TimeEvent*)pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void TimeEventMan::Remove(TimeEvent* pNode)
{
    assert(pNode != nullptr);

    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void TimeEventMan::Dump()
{
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
TimeEventMan* TimeEventMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink* TimeEventMan::derivedCreateNode()
{
    DLink* pNodeBase = new TimeEvent();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

Time TimeEventMan::GetTimeCurrent()
{
    // Get the instance
    TimeEventMan* pMan = TimeEventMan::privGetInstance();
    assert(pMan != nullptr);

    return pMan->mCurrTime;
}

// --- End of File ---
