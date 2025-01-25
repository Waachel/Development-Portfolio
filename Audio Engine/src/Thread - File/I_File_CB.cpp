//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "I_File_CB.h"

using namespace ThreadFramework;

I_File_CB::I_File_CB(bool &DoneFlag)
	: rDoneFlag(DoneFlag)
{

}
	
void I_File_CB::Execute()
{
	//Debug::out("I_File_CB()::Execute()\n");

	this->rDoneFlag = true;

	delete this;
}

//---  End of File ---