//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ASND_MAN_H
#define ASND_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "Snd.h"
#include "ASnd.h"

class ASndMan : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    ASndMan(int reserveNum = 3, int reserveGrow = 1);
    ~ASndMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static ASnd *Add(Snd::ID snd_id, Snd *pSnd);
    static ASnd *Find(Snd::ID snd_id);

    static void Remove(ASnd *pNode);
    static void Dump();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static ASndMan *privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink *derivedCreateNode() override;


    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    ASnd *poNodeCompare;
    static ASndMan *posInstance;

};


#endif
