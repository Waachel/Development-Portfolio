//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "VoiceStitchingDemo.h"
#include "AzulCore.h"
#include "LeftRightPan.h"
#include "SndMan.h"
//#include "Snd.h"
#include "VoiceMan.h"
#include "AudioEngine.h"
#include "Audio.h"
#include "PlaylistData_Stitched.h"
#include "PlaylistJukeBox.h"


using namespace ThreadFramework;
using namespace std::chrono_literals;

void LoadDemo2()
{
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::Intro, "Intro_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::A, "A_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::AtoB, "AtoB_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::B, "B_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::BtoC, "BtoC_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::C, "C_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::CtoA, "CtoA_mono.wav");
	Audio::Load(Audio::Blocking::LOAD, Wave::ID::End, "End_mono.wav");

	PlaylistJukeBox::Load(Snd::ID::Seinfeld, new PlaylistData_Stitched(Wave::ID::Intro, Wave::ID::A, Wave::ID::AtoB, Wave::ID::B, Wave::ID::BtoC, Wave::ID::C, Wave::ID::CtoA, Wave::ID::End));

}

void KillDemo2()
{
	// ---------------------------------------
	// Priority Table
	// ---------------------------------------
	Snd::KillAllActive();
	Snd::PrintPriorityTable();

	// ---------------------------------------
	// Wave Table
	// ---------------------------------------
	Audio::WaveTableDump();
	Audio::RemoveAllWaves();
	Audio::WaveTableDump();
}


void VoiceStitchingMain()
{
	LoadDemo2();
	SimpleBanner b;

	Timer T1;
	T1.tic();


	Handle::Status status;
	Snd* pSndF = SndMan::Add(Snd::ID::Seinfeld);
	status = pSndF->Vol(0.8f);
	status = pSndF->Pan(0.0);
	status = pSndF->Play();

	std::this_thread::sleep_for(10s);

	status = pSndF->Pan(1.0);

	std::this_thread::sleep_for(10s);

	status = pSndF->Pan(-1.0);

	std::this_thread::sleep_for(10s);

	status = pSndF->Pan(0.0);

	std::this_thread::sleep_for(10s);

	status = pSndF->Pan(1.0);

	std::this_thread::sleep_for(10s);

	status = pSndF->Pan(-1.0);

	std::this_thread::sleep_for(1s);
	KillDemo2();
}


//---  End of File ---
