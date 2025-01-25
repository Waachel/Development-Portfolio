//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WAVE_MAN_H
#define WAVE_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Wave.h"
#include "I_File_CB.h"
#include "UserAsyncLoadCallback.h"

class WaveMan : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    WaveMan(int reserveNum = 3, int reserveGrow = 1);
    ~WaveMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static Wave* Add(Wave::ID id, const char* const pWaveName, UserAsyncLoadCallback* pUserAsyncLoadCallback);
    static Wave *Add(Wave::ID id, const char *const pWaveName, I_File_CB *pIFileCB);
    static void Remove(Wave::ID id, I_File_CB *pIFileCB);
    static void RemoveAll(I_File_CB *pIFileCB);
    static Wave *Find(Wave::ID id);

    static void Dump();

    void Remove(Wave *pNode);

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static WaveMan *privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink *derivedCreateNode() override;


    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    Wave *poNodeCompare;
    static WaveMan *posInstance;

};


#endif