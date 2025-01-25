//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "GameMan.h"

int CALLBACK WinMain(HINSTANCE , HINSTANCE ,  LPSTR , int)                  
{          
	// Run unit tests... future testing
	Test::RunTests();

	START_BANNER_MAIN("--Main--");

	// Game is inside a singleton
	GameMan::Create("CSC 588 Engine", 400, 300);

	Game *pGame = GameMan::GetGame();
	pGame->Run();

	GameMan::Destroy();

	return 0;                                       
}

//---  End of File ---
