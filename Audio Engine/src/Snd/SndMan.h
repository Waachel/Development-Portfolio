//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SND_MAN_H
#define SND_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Snd.h"
#include "ASnd.h"
#include "CircularData.h"
#include "Command.h"

class UserSndCallback;

class SndMan : public ManBase
{
public:
    static const Snd::Priority DEFAULT_PRIORITY = 100;


    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    SndMan(int reserveNum = 3, int reserveGrow = 1);
    ~SndMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static Snd *Add(Snd::ID snd_id, UserSndCallback *pUserSndCallback, Snd::Priority);
    static Snd *Add(Snd::ID snd_id, UserSndCallback *pUserSndCallback);
    static Snd *Add(Snd::ID snd_id, Snd::Priority priority);
    static Snd *Add(Snd::ID snd_id);

    static Snd *Find(Snd::ID snd_id);
    static void Remove(Snd *pNode);
    static void Dump();

    static void Update();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static SndMan *privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink *derivedCreateNode() override;

    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    Snd *poNodeCompare;
    static SndMan *posInstance;

};

#endif

// --- End of File ---
