//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WaveMan.h"
#include "Audio_LoadWave_Cmd.h"
#include "SndMan.h"
#include "WaveTable.h"
#include "Audio.h"
#include "File_LoadFile_Cmd.h"
#include "File_AsyncLoadFile_Cmd.h"
#include "QueueMan.h"
#include "Aux_I_RemoveWave_CB_Cmd.h"
#include "Aux_I_RemoveAllWaves_CB_Cmd.h"

using namespace ThreadFramework;

WaveMan *WaveMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
WaveMan::WaveMan(int reserveNum, int reserveGrow)
    : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Wave();
}

WaveMan::~WaveMan()
{
   // Debug::out("~WaveMan()\n");
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Wave *pDeleteMe = (Wave *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while(!pIt->IsDone())
    {
        Wave *pDeleteMe = (Wave *) pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void WaveMan::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new WaveMan(reserveNum, reserveGrow);
    }

}

void WaveMan::Destroy()
{
    WaveMan *pMan = WaveMan::privGetInstance();
    assert(pMan != nullptr);

    delete WaveMan::posInstance;
    WaveMan::posInstance = nullptr;
}

void WaveMan::RemoveAll(I_File_CB *pIFileCB)
{
	WaveMan *pMan = WaveMan::privGetInstance();
	assert(pMan);

	// iterate through the list and delete
	Iterator *pIt = pMan->baseGetActiveIterator();

	DLink *pNode = pIt->First();

	WaveTable *pWaveTable = Audio::GetWaveTable();
	assert(pWaveTable);

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		Wave *pDeleteMe = (Wave *)pIt->Curr();
		pNode = pIt->Next();

		Wave::ID id = pDeleteMe->id;
		pMan->Remove(pDeleteMe);
		pWaveTable->Remove(id);
	}

	// Callback
	assert(pIFileCB);
	Aux_I_RemoveAllWaves_CB_Cmd *pCmd = new Aux_I_RemoveAllWaves_CB_Cmd(pIFileCB);

	//	Debug::out("--> Aux_I_File_CB_Cmd \n");
	bool s = QueueMan::SendAux(pCmd);
	assert(s);

}

void WaveMan::Remove(Wave::ID id, I_File_CB *pIFileCB)
{
	WaveMan *pMan = WaveMan::privGetInstance();
	assert(pMan);

	Wave *pWave = WaveMan::Find(id);
	assert(pWave);

	pMan->Remove(pWave);

	WaveTable *pWaveTable = Audio::GetWaveTable();
	assert(pWaveTable);
	pWaveTable->Remove(id);

	// Callback
	assert(pIFileCB);
	Aux_I_RemoveWave_CB_Cmd *pCmd = new Aux_I_RemoveWave_CB_Cmd(pIFileCB);

	//	Debug::out("--> Aux_I_File_CB_Cmd \n");
	bool s = QueueMan::SendAux(pCmd);
	assert(s);

}

Wave* WaveMan::Add(Wave::ID wave_id, const char* const pWaveName, UserAsyncLoadCallback* pUserAsyncLoadCallback)
{
    WaveMan* pMan = WaveMan::privGetInstance();
    assert(pMan);

    Wave* pNode = (Wave*)pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the data    
    assert(pWaveName);
    assert(pUserAsyncLoadCallback);

    pNode->SetPending(pWaveName, wave_id, pUserAsyncLoadCallback);

    File_AsyncLoadFile_Cmd* pCmd = new File_AsyncLoadFile_Cmd(wave_id, pWaveName, pNode);
    assert(pCmd);

    //  Debug::out("--> File_LoadFile_Cmd \n");
    bool status = QueueMan::SendFile(pCmd);
    assert(status);


    return pNode;
}

Wave * WaveMan::Add(Wave::ID wave_id, const char *const pWaveName, I_File_CB *pIFileCB)
{
    WaveMan *pMan = WaveMan::privGetInstance();
	assert(pMan);

    Wave *pNode = (Wave *) pMan->baseAddToFront();
    assert(pNode != nullptr);

    // Initialize the data    
    assert(pWaveName);
    assert(pIFileCB);
    pNode->SetPending(pWaveName, wave_id, pIFileCB);

    File_LoadFile_Cmd *pCmd = new File_LoadFile_Cmd(wave_id, pWaveName, pNode);
    assert(pCmd);

  //  Debug::out("--> File_LoadFile_Cmd \n");
    bool status = QueueMan::SendFile(pCmd);
    assert(status);


    return pNode;
}

Wave *WaveMan::Find(Wave::ID _id)
{
    WaveMan *pMan = WaveMan::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->id = _id;

    Wave *pData = (Wave *) pMan->baseFind(pMan->poNodeCompare);
    return pData;
}

void WaveMan::Remove(Wave *pNode)
{
    assert(pNode != nullptr);

	this->baseRemove(pNode);
}

void WaveMan::Dump()
{
    WaveMan *pMan = WaveMan::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
WaveMan *WaveMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *WaveMan::derivedCreateNode()
{
    DLink *pNodeBase = new Wave();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

