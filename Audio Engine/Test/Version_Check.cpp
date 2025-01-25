//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "_UnitTestConfiguration.h"
#include "AzulCore.h"
#include "GameApp.h"

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST(Version_Check, TestConfig::ALL)
{
#if Version_Check

	CHECK(GameApp::MAJOR_VERSION == 3);
	CHECK(GameApp::MINOR_VERSION == 1);

#endif
} TEST_END

// ---  End of File ---
