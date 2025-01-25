//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef VOICE_MAN_H
#define VOICE_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Voice.h"
#include "Wave.h"

class VoiceMan : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    VoiceMan(int reserveNum = 3, int reserveGrow = 1);
    ~VoiceMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();
    static Voice *Add(Wave::ID wave_id, VoiceCallback *pCallback);
    static void Remove(Voice *pNode);
    static void Dump();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static VoiceMan *privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink *derivedCreateNode() override;


    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    Voice *poNodeCompare;
    static VoiceMan *posInstance;

};


#endif