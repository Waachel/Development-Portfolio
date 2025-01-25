//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef WAVE_TABLE_H
#define WAVE_TABLE_H

#include "Wave.h"

class WaveTable
{
public:

	static const unsigned int TABLE_COUNT = 10;


	struct Table
	{
		Table();

		Wave::ID       wave_id;
		Wave::Status   status;
	};

public:
	WaveTable();
	WaveTable(const WaveTable &) = delete;
	WaveTable &operator = (const WaveTable &) = delete;
	~WaveTable();

	WaveTable::Table *Find(const Wave::ID wave_id);
	void Register(const Wave::ID wave_id, const Wave::Status status);
	void Update(const Wave::ID wave_id, const Wave::Status status);
	void Remove(const Wave::ID wave_id);
	void Dump();

private:
	Table *privFindEmpty();
	Table *privFind(const Wave::ID _wave_id);

private:
	Table       table[TABLE_COUNT];
	std::mutex  mtx;

};

#endif

//---  End of File ---