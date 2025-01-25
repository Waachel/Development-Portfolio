//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Snd.h"
#include "SndMan.h"
#include "StringThis.h"
#include "Audio_CreateASnd_Cmd.h"
#include "Audio_PlayASnd_Cmd.h"
#include "Audio_StopASnd_Cmd.h"
#include "Audio_VolASnd_Cmd.h"
#include "Audio_PanASnd_Cmd.h"
#include "QueueMan.h"
#include "TimerMan.h"
#include "HandleMan.h"


// Static store
Snd::PriorityEntry  Snd::PriorityTable[PRIORITY_TABLE_SIZE];
std::mutex	        Snd::Table_mtx;

Snd::Snd()
	:snd_id(Snd::ID::Uninitialized),
	priority(0),
	pASnd(nullptr),
	handle()
{
	// nothing dynamic...

	// Pattern:
	//    Manager calls default wave
	//    set(...)  dynamic allocation
	//    privClear(...) release dynamic allocation
	//    destructor release of any owned objects
}

Snd::~Snd()
{
	//Debug::out("~Snd()\n");
}


void Snd::Set(Snd::ID _snd_id, UserSndCallback *pUserSndCallback, Snd::Priority _priority)
{
	this->snd_id = _snd_id;
	this->priority = _priority;

	Handle::Status status = Handle::ActivateHandle(this->handle);
	assert(status == Handle::Status::VALID_HANDLE);

	// Send a command to create ASnd
	Audio_CreateASnd_Cmd *pCmd = new Audio_CreateASnd_Cmd(this->snd_id, this, pUserSndCallback);
	assert(pCmd);

	//Debug::out("--> Audio_CreateASnd_Cmd \n");
	QueueMan::SendAudio(pCmd);
}

void Snd::Set(ASnd *_pASnd)
{
	assert(_pASnd);
	this->pASnd = _pASnd;
}

ASnd *Snd::GetASnd()
{
	assert(pASnd);
	return this->pASnd;
}

void Snd::Dump()
{
	// Dump - Print contents to the debug output window
	Trace::out("\t\tSnd(%p) %s  ASnd(%p)\n", this, StringMe(this->snd_id), this->pASnd);
}

void Snd::privClear()
{
	this->snd_id = Snd::ID::Uninitialized;
	this->priority = 0;
	this->pASnd = nullptr;
	HandleMan::InvalidateHandle(this->handle);
}

void Snd::Wash()
{
	// Wash - clear the entire hierarchy
	DLink::Clear();

	// Sub class clear
	this->privClear();
}

bool Snd::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	Snd *pDataB = (Snd *)pTarget;

	bool status = false;

	if (this->snd_id == pDataB->snd_id)
	{
		status = true;
	}

	return status;
}

void Snd::KillAllActive() 
{
	std::lock_guard<std::mutex> lock(Snd::Table_mtx);

	for (unsigned int i = 0; i < Snd::PRIORITY_TABLE_SIZE; i++)
	{
		Snd *pSnd = Snd::PriorityTable[i].pSnd;
		if (pSnd != nullptr)
		{
			pSnd->Stop();
			Snd::PriorityTable[i].Clear();
		}
	}
}

Handle::Status Snd::Play()
{
	Handle::Lock lock(this->handle);

	Debug::out("Play(%s) priority:%d hdl:0x%x\n", StringMe(this->snd_id), this->priority, this->handle.GetID());

	if (lock)
	{
		if (this->privUseOpenSlotInTable())
		{
			// Easy take a slot
			Audio_PlayASnd_Cmd *pCmd = new Audio_PlayASnd_Cmd(this->snd_id, this);
			assert(pCmd);

			//Debug::out("--> Audio_PlayASnd_Cmd \n");
			QueueMan::SendAudio(pCmd);
		}
		else
		{
			// No - Slots
			// Can I preempt the sound?
			// Table is sorted... so look at last slot only
			if (this->priority <= Snd::PriorityTable[LAST_ENTRY].priority)
			{
				// Priority kill the existing sound
				Snd *pSnd = Snd::PriorityTable[LAST_ENTRY].pSnd;
				assert(pSnd);

				Debug::out("--> Priority Kill(0x%x) \n", pSnd->handle.GetID());

				pSnd->Stop();
				pSnd->RemoveFromPriorityTable();
				//Snd::PrintPriorityTable();

				bool status = this->privUseOpenSlotInTable();
				assert(status);
				
				// Easy take a slot
				Audio_PlayASnd_Cmd *pCmd = new Audio_PlayASnd_Cmd(this->snd_id, this);
				assert(pCmd);

				//Debug::out("--> Audio_PlayASnd_Cmd \n");
				QueueMan::SendAudio(pCmd);
			}
			else
			{
				Debug::out("Reject-->Play(%s) priority:%d hdl:0x%x\n", StringMe(this->snd_id), this->priority, this->handle.GetID());

			}
		}
	}

	return lock;
}

Handle::Status Snd::Stop()
{
	Handle::Lock lock(this->handle);

	if (lock)
	{
		Audio_StopASnd_Cmd *pCmd = new Audio_StopASnd_Cmd(this->snd_id, this);
		assert(pCmd);

		//Debug::out("--> Audio_StopASnd_Cmd \n");
		QueueMan::SendAudio(pCmd);
	}

	return lock;
}

Handle::Status Snd::Vol(float vol)
{
	Handle::Lock lock(this->handle);

	if (lock)
	{
		Audio_VolASnd_Cmd *pCmd = new Audio_VolASnd_Cmd(this->snd_id, this, vol);
		assert(pCmd);

		//Debug::out("--> Audio_VolASnd_Cmd \n");
		QueueMan::SendAudio(pCmd);
	}
	else
	{
		assert(false);
	}

	return lock;
}

Handle::Status Snd::Pan(float pan)
{
	Handle::Lock lock(this->handle);

	if (lock)
	{
		Audio_PanASnd_Cmd *pCmd = new Audio_PanASnd_Cmd(this->snd_id, this, pan);
		assert(pCmd);

		//Debug::out("--> Audio_PanASnd_Cmd \n");
		QueueMan::SendAudio(pCmd);
	}

	return lock;
}

Snd::PriorityEntry::PriorityEntry()
{
	this->Clear();
}

void Snd::PriorityEntry::Clear()
{
	this->priority = -1;
	this->sndID = Snd::ID::Uninitialized;
	this->handleID = 0;
	this->startTime = Time(Duration::TIME_ZERO);
	this->pSnd = nullptr;
}

void Snd::RemoveFromPriorityTable()
{
	std::lock_guard<std::mutex> lock(Snd::Table_mtx);

	bool status = false;

	for (unsigned int i = 0; i < Snd::PRIORITY_TABLE_SIZE; i++)
	{
		if (Snd::PriorityTable[i].handleID == this->handle.GetID())
		{
			Snd::PriorityTable[i].Clear();
			status = true;
			break;
		}
	}

	Snd::SortPriorityTable();
}

#include <algorithm>

bool sortFunc(Snd::PriorityEntry a, Snd::PriorityEntry b)
{
	if (a.priority < b.priority)
	{
		return true;
	}
	else if (a.priority == b.priority)
	{
		if (a.startTime > b.startTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Snd::SortPriorityTable()
{
	std::sort(std::begin(Snd::PriorityTable),
		std::end(Snd::PriorityTable),
		sortFunc);
}

bool Snd::privUseOpenSlotInTable()
{
	std::lock_guard<std::mutex> lock(Snd::Table_mtx);

	bool status = false;

	// Find open slot?
	for (unsigned int i = 0; i < Snd::PRIORITY_TABLE_SIZE; i++)
	{
		if (PriorityTable[i].priority == -1)
		{
			TimerMan::Update();
			PriorityTable[i].priority = this->priority;
			PriorityTable[i].sndID = this->snd_id;
			PriorityTable[i].handleID = this->handle.GetID();
			PriorityTable[i].startTime = TimerMan::GetTimeCurrent();
			PriorityTable[i].pSnd = this;

			// Found one
			status = true;
			break;
		}
	}

	Snd::SortPriorityTable();

	return status;
}

Time Snd::GetTime()
{
	std::lock_guard<std::mutex> lock(Snd::Table_mtx);

	Time startTime;
	bool status = false;
	for (unsigned int i = 0; i < Snd::PRIORITY_TABLE_SIZE; i++)
	{
		if (Snd::PriorityTable[i].handleID == this->handle.GetID())
		{
			startTime = PriorityTable[i].startTime;
			status = true;
			break;
		}
	}

	assert(status);

	return startTime;
}

void Snd::PrintPriorityTable()
{
	std::lock_guard<std::mutex> lock(Snd::Table_mtx);

	size_t count = 512;
	char buff[512];
	char *pBuff = buff;
	int offset;

	offset = sprintf_s(pBuff, count, "---------------------------------\n");
	pBuff += offset;
	count -= offset;
	offset = sprintf_s(pBuff, count, " Snd Priority Table \n");
	pBuff += offset;
	count -= offset;
	offset = sprintf_s(pBuff, count, "---------------------------------\n");
	pBuff += offset;
	count -= offset;

	for (unsigned int i = 0; i < Snd::PRIORITY_TABLE_SIZE; i++)
	{

		if (Snd::PriorityTable[i].priority != -1)
		{
			Time delta = (TimerMan::GetTimeCurrent()) - PriorityTable[i].startTime;

			offset = sprintf_s(pBuff,
				count,
				"hdl: 0x%x %-15s prior:%4d  time: %d ms\n",
				PriorityTable[i].handleID,
				StringMe(PriorityTable[i].sndID),
				PriorityTable[i].priority,
				Time::quotient(delta, Time(Duration::TIME_ONE_MILLISECOND)));
		}
		else
		{
			offset = sprintf_s(pBuff,
				count,
				"hdl: 0x-------- Snd::---------- prior: ---  time: --- \n");
		}

		pBuff += offset;
		count -= offset;
	}

	sprintf_s(pBuff, count, "---------------------------------\n");
	pBuff += offset;
	count -= offset;

	Trace::out(buff);
}

// --- End of File ---
