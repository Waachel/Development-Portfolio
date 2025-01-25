//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TIME_EVENT_H
#define	TIME_EVENT_H

#include "DLink.h"
#include "Command.h"
#include "AzulTime.h"

class TimeEvent : public DLink
{
public:
	enum ID
	{
		AsyncDemo = 0x66660000,
		Uninitialized
	};

public:

	TimeEvent();
	TimeEvent(const TimeEvent&) = delete;
	TimeEvent& operator = (const TimeEvent&) = delete;
	virtual ~TimeEvent();

	void SetID(ID event_id);
	TimeEvent::ID GetID();

	Time GetTriggerTime();
	void Process();

	void Set(Command* pCommand, Time deltaTimeToTrigger);

	bool Compare(DLink* pTarget) override;
	void Dump();
	void Wash();


private:
	void PrivClear();

	//-------------------------------------------
	// Data:  public for now
	//-------------------------------------------

private:
	Command* pCommand;
	ID		id;

	Time	TriggerTime;
	Time	DeltaTime;
};


#endif

// --- End of File ---
