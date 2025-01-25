//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "AzulCore.h"
#include "HandleMan.h"
#include "Game.h"
#include "GameApp.h"
#include "InputTest.h"
#include "AudioThread.h"
#include "TestThread.h"
#include "PriorityTableDemo.h"
#include "VoiceStitchingDemo.h"
#include "UserCallbacksDemo.h"
#include "BasicsDemo.h"
#include "AsyncDemo.h"
#include "CircularData.h"
#include "InputMan.h"
#include "SndMan.h"
#include "Snd.h"
#include "Audio_LoadWave_Cmd.h"
#include "PlaylistMan.h"
#include "PlaylistJukeBox.h"
#include "WaveMan.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "PlaylistData_One.h"
#include "PlaylistData_Stitched.h"
#include "Audio.h"
#include "QueueMan.h"
#include "TimerMan.h"
#include "UserSndCallBack.h"
#include "TimeEventMan.h"

// Global varables --- todo remove

// Needs to be an atomic
std::atomic_bool QuitFlag = false;
std::atomic_bool AudioReadyFlag = false;
int Demo1 = 0;
int Demo2 = 0;
int Demo3 = 0;
int Demo4 = 0;
int Demo5 = 0;
using namespace ThreadFramework;
using namespace std::chrono_literals;


//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char *const pWindowName, const int Width, const int Height)
	:Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
	this->globalTimer.tic();
	this->intervalTimer.tic();
}

//-----------------------------------------------------------------------------
//  Game::~Game()
//		Game Engine destructor
//-----------------------------------------------------------------------------
Game::~Game()
{
}

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{

}

//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{	
	// Load up the demo application
	GameApp::LoadDemo(this->info.windowWidth, this->info.windowHeight);

	HandleMan::Create();
	SndMan::Create();
	InputMan::Create();
	Audio::Create();
	QueueMan::Create();
	TimerMan::Create();
	TimeEventMan::Create();

	//--------------------------------------------------------------------
	// Launch a Thread
	//--------------------------------------------------------------------

	// Spawn Audio thread
	std::thread  t_Audio(AudioMain, std::ref(QuitFlag), std::ref(AudioReadyFlag));
	Debug::SetName(t_Audio, "--Audio--");
	t_Audio.detach();	

	// Wait until audio thread is ready.
	while(!AudioReadyFlag);

}

//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------

void Game::Update(float)
{
	// Update the demo application
	GameApp::UpdateDemo();
	TimerMan::Update();
	SndMan::Update();
	TimeEventMan::Update();

	// Read Quit Key
	Keyboard *key = InputMan::GetKeyboard();
	if(key->GetKeyState(Keyboard::KEY_Q))
	{
		QuitFlag = true;
	}

	//Demo 1: Basics
	if (key->GetKeyState(Keyboard::KEY_1))
	{
		if (Demo1 == 0)
		{
			// Test thread
			std::thread t1(BasicsDemoMain);
			Debug::SetName(t1, "--Basics Demo--");
			t1.detach();

			Demo1++;
		}
	}

	//Demo 2: Voice Stitching
	if (key->GetKeyState(Keyboard::KEY_2))
	{
		if (Demo2 == 0)
		{
			// Test thread
			std::thread t1(VoiceStitchingMain);
			Debug::SetName(t1, "--Voice Stitching Demo--");
			t1.detach();

			Demo2++;
		}
	}

	//Demo 3: Priority
	if (key->GetKeyState(Keyboard::KEY_3))
	{
		if (Demo3 == 0)
		{
			// Test thread
			std::thread t1(PriorityTableDemoMain);
			Debug::SetName(t1, "--Priority Demo--");
			t1.detach();

			Demo3++;
		}
	}

	//Demo 4: User Callbacks
	if (key->GetKeyState(Keyboard::KEY_4))
	{
		if (Demo4 == 0)
		{
			// Test thread
			std::thread t1(UserCallbacksDemoMain);
			Debug::SetName(t1, "--User Callbacks Demo--");
			t1.detach();

			Demo4++;
		}
	}

	//Demo 5: Async Loading
	if (key->GetKeyState(Keyboard::KEY_SPACE))
	{
		if (Demo5 == 0)
		{
			// Test thread
			std::thread t1(AsyncDemoMain);
			Debug::SetName(t1, "--Async Demo--");
			t1.detach();

			Demo5++;
		}
	}

}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	// Draw the demo application
	GameApp::DrawDemo();
}

//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	InputMan::Destroy();
	SndMan::Destroy();
	HandleMan::Destroy();

	Audio::Destroy();
	QueueMan::Destroy();
	TimerMan::Destroy();
	TimeEventMan::Destroy();
}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.9f, 0.8f, 0.8f, 1.0f };
	constexpr GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

//---  End of File ---
