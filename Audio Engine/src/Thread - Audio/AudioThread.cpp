//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "VoiceCallback.h"
#include "AudioThread.h"
#include "XAudio2Wrapper.h"
#include "AudioEngine.h"
#include "Voice.h"
#include "VoiceMan.h"
#include "Wave.h"
#include "WaveMan.h"
#include "SndMan.h"
#include "PlaylistMan.h"
#include "PlaylistJukeBox.h"
#include "ASndMan.h"
#include "FileThread.h"
#include "QueueMan.h"
#include "AuxThread.h"

using namespace ThreadFramework;


void AudioMain(std::atomic_bool &QuitFlag, std::atomic_bool &AudioReadyFlag)
{
	SimpleBanner b;
	
	// Spawn Audio thread
	std::thread  t_File(FileMain, std::ref(QuitFlag));
	Debug::SetName(t_File, "--File--",2);
	t_File.detach();
	
	// Spawn Aux thread
	std::thread  t_Aux(AuxMain, std::ref(QuitFlag));
	Debug::SetName(t_Aux, "--Aux --", 2);
	t_Aux.detach();

	// Create the audio engine... 
	AudioEngine engine;

	WaveMan::Create();
	VoiceMan::Create();
	PlaylistJukeBox::Create();
	PlaylistMan::Create();
	ASndMan::Create();

	CircularData *pAudioIn = QueueMan::GetAudioInQueue();
	
	AudioReadyFlag = true;

	// ----------------------------------------
	// Loop for ever until quit is hit
	// ----------------------------------------
	while(!QuitFlag)
	{
		Command *pCmd;

		if(pAudioIn->PopFront(pCmd) == true)
		{
			assert(pCmd);
			pCmd->Execute();
		}
	}
	
	ASndMan::Destroy();
	PlaylistMan::Destroy();
	PlaylistJukeBox::Destroy();
	VoiceMan::Destroy();
	WaveMan::Destroy();
	
}

//---  End of File ---
