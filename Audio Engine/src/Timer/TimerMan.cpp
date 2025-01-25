//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "TimerMan.h"

TimerMan *TimerMan::posInstance = nullptr;

TimerMan::TimerMan()
{
    this->mGameTimer.tic();
    this->mCurrTime = this->mGameTimer.toc();
}

TimerMan::~TimerMan()
{

}

void TimerMan::Destroy()
{
    TimerMan *pTimerMan = TimerMan::privGetInstance();
    assert(pTimerMan != nullptr);

    delete TimerMan::posInstance;
    TimerMan::posInstance = nullptr;
}

void TimerMan::Create()
{
    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new TimerMan();
    }
}

TimerMan *TimerMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

void TimerMan::Update()
{
    TimerMan *pTimerMan = TimerMan::privGetInstance();
    assert(pTimerMan != nullptr);

    pTimerMan->mCurrTime = pTimerMan->mGameTimer.toc();
}

Time TimerMan::GetTimeCurrent()
{
    TimerMan *pTimerMan = TimerMan::privGetInstance();
    assert(pTimerMan != nullptr);

    return pTimerMan->mCurrTime;
}


// --- End of File ---
