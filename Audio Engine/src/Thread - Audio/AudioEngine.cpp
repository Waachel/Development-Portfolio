//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AudioEngine.h"
#include "VoiceCallback.h"

using namespace ThreadFramework;

AudioEngine *AudioEngine::psInstance = nullptr;

AudioEngine::AudioEngine()
	: poXAudio2(nullptr),
	poMasterVoice(nullptr)
{
	//Debug::out("AudioEngine::AudioEngine() \n");
	this->privStartCOMThreads();
	this->privCreateXAudio2();
	this->privCreateMasterVoice();

	assert(AudioEngine::psInstance == nullptr);
	AudioEngine::psInstance = this;
}

IXAudio2MasteringVoice *AudioEngine::GetMasterVoice()
{
	AudioEngine *pAudio = AudioEngine::privGetInstance();
	assert(pAudio);

	return pAudio->poMasterVoice;
}

IXAudio2 *AudioEngine::GetXAudio2()
{
	AudioEngine *pAudio = AudioEngine::privGetInstance();
	assert(pAudio);

	return pAudio->poXAudio2;
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
AudioEngine *AudioEngine::privGetInstance()
{
	return AudioEngine::psInstance;
}

AudioEngine::~AudioEngine()
{
	//Debug::out("AudioEngine::~AudioEngine() \n");
	assert(this->poMasterVoice);
	assert(this->poXAudio2);

	// Destroy this->poMasterVoice;
	// this was created in XAudio2... so that system should release
	this->poMasterVoice->DestroyVoice();

	// Destroy this->poXAudio2;
	// this was created in XAudio2... so that system should release
	this->poXAudio2->Release();

	this->privEndCOMThreads();
}


void AudioEngine::privStartCOMThreads()
{
	// Initializes the COM library for use by the calling thread
	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(hr == S_OK);
}

void AudioEngine::privEndCOMThreads()
{
	// Closes the COM library on the current thread
	CoUninitialize();
}

void AudioEngine::privCreateXAudio2()
{
	// -------------------------------------------------------
	// Create an instance, use its default processor
	// -------------------------------------------------------
	HRESULT hr;
	hr = XAudio2Create(&this->poXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(hr == S_OK);

	// test it
	assert(this->poXAudio2);
}

void AudioEngine::privCreateMasterVoice()
{
	// -------------------------------------------------------
	// Creating a master voice, with default settings:
	//
	//      InputChannels = XAUDIO2_DEFAULT_CHANNELS,
	//      InputSampleRate = XAUDIO2_DEFAULT_SAMPLERATE,
	//      Flags = 0,
	//      szDeviceId = NULL,
	//      *pEffectChain = NULL,
	//      StreamCategory = AudioCategory_GameEffects
	// -------------------------------------------------------

	HRESULT hr;
	hr = this->poXAudio2->CreateMasteringVoice(&this->poMasterVoice);
	assert(hr == S_OK);

	// test it
	assert(this->poMasterVoice);
}

/*void AudioEngine::StartSound(Voice *pVoice)
{
	// -------------------------------------------------------
	// Start the source voice
	// -------------------------------------------------------
	assert(pVoice);
	Handle::Status status;
	status = pVoice->Start();
	assert(status == Handle::Status::SUCCESS);
}

void AudioEngine::StopSound(Voice* pVoice)
{
	// -------------------------------------------------------
	// Stop the source voice
	// -------------------------------------------------------
	assert(pVoice);
	Handle::Status status;
	status = pVoice->Stop();
	assert(status == Handle::Status::SUCCESS);
}

*/
// --- End of File ---
