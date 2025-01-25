//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TIME_EVENT_MAN_H
#define TIME_EVENT_MAN_H 

#include "ManBase.h"
#include "DLinkMan.h"
#include "TimeEvent.h"
#include "AzulTimer.h"

class TimeEventMan : public ManBase
{

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
private:
    TimeEventMan(int reserveNum = 3, int reserveGrow = 1);
    ~TimeEventMan();

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static TimeEvent* Add(Command* pCommand, Time deltaTimeToTrigger);
    static TimeEvent* Find(TimeEvent::ID id);
    static Time GetTimeCurrent();

    static void Update();
    static void UpdateTimeOnly();
    static void Reset();

    static void Remove(TimeEvent* pNode);
    static void Dump();

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
private:
    static TimeEventMan* privGetInstance();

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
protected:
    DLink* derivedCreateNode() override;


    //----------------------------------------------------------------------
    // Data: unique data for this manager 
    //----------------------------------------------------------------------
private:
    TimeEvent* poNodeCompare;
    static TimeEventMan* posInstance;

    Time		mCurrTime;
    Timer		GameTime;

};


#endif