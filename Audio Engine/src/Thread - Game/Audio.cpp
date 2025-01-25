//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "Audio.h"
#include "Audio_LoadWave_Cmd.h"
#include "Audio_LoadAsyncWave_Cmd.h"
#include "SndMan.h"
#include "I_File_CB.h"
#include "QueueMan.h"
#include "WaveMan.h"
#include "Audio_RemoveWave_Cmd.h"
#include "Audio_RemoveAllWaves_Cmd.h"
#include "StringThis.h"

Audio *Audio::posInstance = nullptr;

Audio::Audio()
{
    this->poWaveTable = new WaveTable();
    assert(this->poWaveTable);
}

Audio::~Audio()
{
    delete this->poWaveTable;
}

void Audio::Destroy()
{
    Audio *pAudio = Audio::privGetInstance();
    assert(pAudio != nullptr);

    delete Audio::posInstance;
    Audio::posInstance = nullptr;
}

void Audio::Create()
{

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new Audio();
    }

}

void Audio::WaveTableDump()
{
    Audio *pAudio = Audio::privGetInstance();
    pAudio->poWaveTable->Dump();
}

void Audio::Remove(Wave::ID wave_id)
{
	Audio *pAudio = Audio::privGetInstance();
	assert(pAudio);

	Wave *pWave;
	pWave = WaveMan::Find(wave_id);
	assert(pWave);

	// Setup the Callback
	bool DoneFlag = false;
	I_File_CB *pFileCB = new I_File_CB(DoneFlag);

	Audio_RemoveWave_Cmd *pCmd = new Audio_RemoveWave_Cmd(wave_id, pFileCB);
	assert(pCmd);

	Debug::out("--> Audio_RemoveWave_Cmd(%s) \n", StringMe(wave_id));
	QueueMan::SendAudio(pCmd);

	// Block and spin until Callback
	while (!DoneFlag);

}


void Audio::RemoveAllWaves()
{
	Audio *pAudio = Audio::privGetInstance();
	assert(pAudio);

	// Setup the Callback
	bool DoneFlag = false;
	I_File_CB *pFileCB = new I_File_CB(DoneFlag);

	Audio_RemoveAllWaves_Cmd *pCmd = new Audio_RemoveAllWaves_Cmd(pFileCB);
	assert(pCmd);

	Debug::out("--> Audio_RemoveWave_Cmd() \n");
	QueueMan::SendAudio(pCmd);

	// Block and spin until Callback
	while (!DoneFlag);

}

void Audio::Load(Async,
    Wave::ID wave_id,
    const char* const pWaveName,
    UserAsyncLoadCallback* pUserAsyncLoadCallback)
{
    assert(pWaveName);
    Debug::out("Audio::Load-Async(%s)\n", pWaveName);

    Audio* pAudio = Audio::privGetInstance();

    //Is Wave in the WaveTable?
    WaveTable* pWaveTable = pAudio->poWaveTable;
    assert(pWaveTable);
    WaveTable::Table* pTable = pWaveTable->Find(wave_id);

    // Not there...
    if (pTable == nullptr)
    {
        pWaveTable->Register(wave_id, Wave::Status::PENDING);

        assert(pWaveName);

        // Setup the Callback
        assert(pUserAsyncLoadCallback);
        pUserAsyncLoadCallback->Set(wave_id, pWaveName);

        Audio_LoadAsyncWave_Cmd* pCmd = new Audio_LoadAsyncWave_Cmd(wave_id, pWaveName, pUserAsyncLoadCallback);
        assert(pCmd);

        //   Debug::out("--> Audio_LoadAsyncWave_Cmd \n");
        QueueMan::SendAudio(pCmd);

    }
    else
    {
        // its Ready or Pending...
        if (pTable->status == Wave::Status::READY || pTable->status == Wave::Status::PENDING)
        {
            // Do nothing... 
        }
        else
        {
            // bad...
            assert(false);
        }
    }

}

void Audio::Load(Blocking,
	Wave::ID wave_id,
	const char *const pWaveName)
{
	assert(pWaveName);
	Debug::out("Audio::Load-Blocking(%s)\n", pWaveName);

    Audio *pAudio = Audio::privGetInstance();

    //Is Wave in the WaveTable?
    WaveTable *pWaveTable = pAudio->poWaveTable;
    assert(pWaveTable);
    WaveTable::Table *pTable = pWaveTable->Find(wave_id);
    
    // Not there...
    if(pTable == nullptr)
    {
        pWaveTable->Register(wave_id, Wave::Status::PENDING);

        assert(pWaveName);

        // Setup the Callback
        bool DoneFlag = false;
        I_File_CB *pFileCB = new I_File_CB(DoneFlag);

        Audio_LoadWave_Cmd *pCmd = new Audio_LoadWave_Cmd(wave_id, pWaveName, pFileCB);
        assert(pCmd);

     //   Debug::out("--> Audio_LoadWave_Cmd \n");
        QueueMan::SendAudio(pCmd);


        // Block and spin until Callback
        while(!DoneFlag);

    }
    else
    {
        // its Ready or Pending...
        if(pTable->status == Wave::Status::READY || pTable->status == Wave::Status::PENDING)
        {
            // Do nothing... 
        }
        else
        {
            // bad...
            assert(false);
        }
    }

}

Audio *Audio::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

WaveTable * Audio:: GetWaveTable()
{
    Audio *pAudio = Audio::privGetInstance();
    return pAudio->poWaveTable;
}

// --- End of File ---