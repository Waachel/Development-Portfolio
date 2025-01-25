//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------


#ifndef SND_H
#define SND_H

#include "Handle.h"
#include "DLink.h"
#include "Snd.h"
#include "AzulTimer.h"

class ASnd;
class UserSndCallback;

class Snd : public DLink
{
public:
	enum class ID : uint32_t
	{
		Fiddle = 101,
		Bassoon = 102,
		Oboe2_mono = 103,
		SongA = 104,
		SongB = 105,
		Seinfeld = 201,
		Strings,
		Calliope,
		Oboe,
		Coma = 301,
		Dial = 401,
		MoonPatrol = 402,
		Sequence = 403,
		Donkey = 404,
		Alarm = 501,
		Electro = 502,
		Beethoven = 503,
		Uninitialized
	};

	typedef int Priority;

public:
	// constants
	static const unsigned int PRIORITY_TABLE_SIZE = 6;
	static const unsigned int LAST_ENTRY = PRIORITY_TABLE_SIZE-1;
	static const int PRIORITY_TABLE_INVALID = -1;	

public:

	class PriorityEntry
	{
	public:
		PriorityEntry();

		void Clear();

		// Data:
		Handle::ID	handleID;
		Snd::ID		sndID;
		int			priority;
		Time		startTime;
		Snd			*pSnd;
	};

public:
	// Big 4
	Snd();
	Snd(const Snd &) = delete;
	Snd &operator = (const Snd &) = delete;
	virtual ~Snd();

	void Set(Snd::ID snd_id, UserSndCallback *pUserSndCallback, Snd::Priority priority);
	void Dump();
	void Wash();
	virtual bool Compare(DLink *pTargetNode) override;

	void Set(ASnd *pASnd);
	ASnd *GetASnd();

	Time GetTime();

	Handle::Status Play();
	Handle::Status Stop();
	Handle::Status Vol(float v);
	Handle::Status Pan(float p);

	void RemoveFromPriorityTable();

	static void KillAllActive();
	static void SortPriorityTable();
	static void PrintPriorityTable();

private:
	void privClear();
	bool privUseOpenSlotInTable();

public:
	//----------------------------------------------------
	// Data
	//----------------------------------------------------
	Snd::ID snd_id;
	Snd::Priority priority;
	ASnd *pASnd;

	Handle handle;

	//-----------------------------------------------------
	// Table
	//-----------------------------------------------------

	// ------- STATIC ----------------
	static PriorityEntry  PriorityTable[PRIORITY_TABLE_SIZE];
	static std::mutex	  Table_mtx;

};

#endif
