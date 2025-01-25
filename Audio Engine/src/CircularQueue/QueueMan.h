//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef QUEUE_MAN_H
#define QUEUE_MAN_H

#include "CircularData.h"

class QueueMan
{
public:
	static void Create();
	static void Destroy();

	static bool SendAudio(Command *pCmd);
	static bool SendFile(Command *pCmd);
	static bool SendGame(Command *pCmd);
	static bool SendAux(Command *pCmd);

	static CircularData *GetGameInQueue();
	static CircularData *GetAudioInQueue();
	static CircularData *GetFileInQueue();
	static CircularData *GetAuxInQueue();

private:
	QueueMan();
	QueueMan(const QueueMan &) = delete;
	QueueMan &operator = (const QueueMan &) = delete;
	~QueueMan();

	static QueueMan *privGetInstance();
	static QueueMan *posInstance;

	CircularData *poAudioIn; // ---> to Audio
	CircularData *poGameIn;  // ---> to Game
	CircularData *poFileIn;  // ---> to File
	CircularData *poAuxIn;   // ---> to Aux

};

#endif

//---  End of File ---
