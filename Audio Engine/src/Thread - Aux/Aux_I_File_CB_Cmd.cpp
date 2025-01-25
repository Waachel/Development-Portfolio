//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

using namespace ThreadFramework;

#include "Aux_I_File_CB_Cmd.h"

Aux_I_File_CB_Cmd::Aux_I_File_CB_Cmd(I_File_CB *_pIFileCB)
	: pIFileCB(_pIFileCB)
{
	assert(pIFileCB);
}

void Aux_I_File_CB_Cmd::Execute()
{
//	Debug::out("Aux_I_File_CB_Cmd::Execute()\n");

	this->pIFileCB->Execute();

	delete this;
}

// --- End of File ---
