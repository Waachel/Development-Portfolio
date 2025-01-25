//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "QueueMan.h"


QueueMan *QueueMan::posInstance = nullptr;

QueueMan::QueueMan()
{
    // Shared Queues
    poAudioIn = new CircularData();
    assert(poAudioIn);

    poGameIn = new CircularData();
    assert(poGameIn);

    poFileIn = new CircularData();
    assert(poFileIn);

    poAuxIn = new CircularData();
    assert(poFileIn);
}

QueueMan::~QueueMan()
{
    delete this->poAudioIn;
    this->poAudioIn = nullptr;

    delete this->poGameIn;
    this->poGameIn = nullptr;

    delete this->poFileIn;
    this->poFileIn = nullptr;

    delete this->poAuxIn;
    this->poAuxIn = nullptr;
}

void QueueMan::Destroy()
{
    QueueMan *pQueueMan = QueueMan::privGetInstance();
    assert(pQueueMan != nullptr);

    delete QueueMan::posInstance;
    QueueMan::posInstance = nullptr;
}

void QueueMan::Create()
{
    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if(posInstance == nullptr)
    {
        posInstance = new QueueMan();
    }
}


QueueMan *QueueMan::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}



bool QueueMan::SendAudio(Command *pCmd)
{
    QueueMan *pMan = QueueMan::privGetInstance();

    bool status = pMan->poAudioIn->PushBack(pCmd);

    return status;
}

bool QueueMan::SendGame(Command *pCmd)
{
    QueueMan *pMan = QueueMan::privGetInstance();

    bool status = pMan->poGameIn->PushBack(pCmd);

    return status;
}

bool QueueMan::SendFile(Command *pCmd)
{
    QueueMan *pMan = QueueMan::privGetInstance();

    bool status = pMan->poFileIn->PushBack(pCmd);

    return status;
}

bool QueueMan::SendAux(Command *pCmd)
{
    QueueMan *pMan = QueueMan::privGetInstance();

    bool status = pMan->poAuxIn->PushBack(pCmd);

    return status;
}


CircularData *QueueMan::GetGameInQueue()
{
    QueueMan *pMan = QueueMan::privGetInstance();
    return pMan->poGameIn;
}

CircularData *QueueMan::GetAudioInQueue()
{
    QueueMan *pMan = QueueMan::privGetInstance();
    return pMan->poAudioIn;
}

CircularData *QueueMan::GetFileInQueue()
{
    QueueMan *pMan = QueueMan::privGetInstance();
    return pMan->poFileIn;
}

CircularData *QueueMan::GetAuxInQueue()
{
    QueueMan *pMan = QueueMan::privGetInstance();
    return pMan->poAuxIn;
}


// --- End of File ---
