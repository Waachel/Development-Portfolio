//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ASndMan.h"
#include "SndMan.h"
#include "Playlist.h"

using namespace ThreadFramework;

ASndMan *ASndMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ASndMan::ASndMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new ASnd();
}

ASndMan::~ASndMan()
{
    // Debug::out("~ASnd()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        ASnd *pDeleteMe = (ASnd *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        ASnd *pDeleteMe = (ASnd *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void ASndMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new ASndMan(reserveNum, reserveGrow);
    }

}

void ASndMan::Destroy()
{
    ASndMan *pMan = ASndMan::privGetInstance();
    assert(pMan != nullptr);

    delete ASndMan::posInstance;
    ASndMan::posInstance = nullptr;
}

ASnd *ASndMan::Add(Snd::ID snd_id, Snd *pSnd)
{
    ASndMan *pMan = ASndMan::privGetInstance();

    ASnd *pNode = (ASnd *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the date
    assert(pSnd);
    pNode->Set(snd_id, pSnd);

    return pNode;
}

ASnd *ASndMan::Find(Snd::ID _id)
{
    ASndMan *pMan = ASndMan::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->snd_id = _id;

    ASnd *pData = (ASnd *) pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void ASndMan::Remove(ASnd *pNode)
{
    assert(pNode != nullptr);

    ASndMan *pMan = ASndMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void ASndMan::Dump()
{
    ASndMan *pMan = ASndMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
ASndMan *ASndMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *ASndMan::derivedCreateNode()
{
    DLink *pNodeBase = new ASnd();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

