//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "XAudio2Wrapper.h"

#include "AudioEngine.h"
#include "Voice.h"
#include "StringThis.h"

Voice::Voice()
	:pWave(nullptr),
	poBuff(nullptr),
	porCallback(nullptr),
	poSourceVoice(nullptr),
	handle()
{
	this->poBuff = new XAUDIO2_BUFFER_ALIAS();
	assert(this->poBuff);

	//Debug::out("voice(): %p\n", this);
}

void Voice::Set(Wave *_pWave, VoiceCallback *pCallback)
{
	//Debug::out("voice::set(): %p\n", this);
	Handle::Status s;

	s = Handle::ActivateHandle(this->handle);
	assert(s == Handle::Status::VALID_HANDLE);

	assert(_pWave);
	this->pWave = _pWave;

	// transfer ownership
	assert(pCallback);
	this->porCallback = pCallback;

	IXAudio2 *pXAudio2 = AudioEngine::GetXAudio2();
	assert(pXAudio2);

	// Create Source Voice
		this->poSourceVoice = nullptr;
		HRESULT hr;
		hr = pXAudio2->CreateSourceVoice(&this->poSourceVoice, (WAVEFORMATEX *)this->pWave->poWfx, 0, XAUDIO2_MAX_FREQ_RATIO, this->porCallback);
		assert(hr == S_OK);
		assert(this->poSourceVoice);

	// SourceBuffer
		assert(this->poBuff);
		if (pCallback->type == VoiceCallback::Type::One)
		{
			this->privInitSrcBuff(true);
		}
		else
		{
			this->privInitSrcBuff(false);
		}

		hr = this->poSourceVoice->SubmitSourceBuffer((XAUDIO2_BUFFER *) this->poBuff);
		assert(hr == S_OK);
		assert(this->poBuff);
}

Voice::~Voice()
{
	// ------------------------------------------------------------------------
	//   Handle is invalidated automatically - through ~handle() destructor
	// ------------------------------------------------------------------------

	//Debug::out("~Voice(%p)\n", this);

	// This was initialized in XAudio2... so its destroyed there

	// Protection for empty voices
	if(this->poSourceVoice)
	{
		this->poSourceVoice->Stop();
		this->poSourceVoice->DestroyVoice();
		this->poSourceVoice = nullptr;
	}

	if(this->poBuff)
	{
		// Done in the wave manager
			//delete this->poBuff->pAudioData;
		delete this->poBuff;
		this->poBuff = nullptr;
	}

	if(this->porCallback)
	{
		delete this->porCallback;
		this->porCallback = nullptr;
	}

	// check "this" and input
	// OK - its invalidated in the manager
	// assert(Handle::IsValid(this->handle) == Handle::Status::VALID_HANDLE);

}

void Voice::privInitSrcBuff(bool StreamEnd)
{
	assert(this->poBuff);
	assert(this->pWave);

	*poBuff = {0};

	poBuff->AudioBytes = pWave->rawBuffSize; //buffer containing audio data
	poBuff->pAudioData = pWave->poRawBuff;   //size of the audio buffer in bytes
	poBuff->Flags = 0;   // tell the source voice not to expect any data after this buffer

	if (StreamEnd)
	{
		poBuff->Flags = XAUDIO2_END_OF_STREAM;   // tell the source voice not to expect any data after this buffer
	}
	else
	{
		poBuff->Flags = 0;   // tell the source voice not to expect any data after this buffer

	}
	poBuff->LoopCount = 0;				     // looping...

}

// Manager methods
void Voice::Wash()
{
	this->Clear();
}

void Voice::Dump()
{
	if(this->pWave)
	{
		Trace::out("\t\tVoice(%p): %s \n", this, StringMe(this->pWave->GetId()));
	}
	else
	{
		Trace::out("\t\tVoice(%p): --- \n", this);
	}
}

bool Voice::Compare(DLink *)
{
	// not implemented
	assert(false);

	return false;
}


void Voice::Clear()
{
	assert(this->poBuff);
	*this->poBuff = {0};
	
	if(this->porCallback)
	{

		delete this->porCallback;	

		this->porCallback = nullptr;
	}

	if(this->poSourceVoice)
	{
		this->poSourceVoice->Stop();
		this->poSourceVoice->DestroyVoice();
		this->poSourceVoice = nullptr;
	}

	//Debug::out(" voice::clear()  %p\n", this);
	// validate when used in Set

	Handle::Status s;

	s = Handle::InvalidateHandle(this->handle);
	assert(s == Handle::Status::INVALID_HANDLE);
}


Handle::Status Voice::Start()
{
	Handle::Lock lock(this->handle);

	if(lock)
	{
		HRESULT hr;
	//	hr = poSourceVoice->SetVolume(0.1f);
	//	assert(hr == S_OK);
		hr = poSourceVoice->Start(0);
		assert(hr == S_OK);
	}

	return lock;
}


Handle::Status  Voice::Stop()
{
	Handle::Lock lock(this->handle);

	if(lock)
	{
		HRESULT hr;
		hr = poSourceVoice->Stop(0);
		assert(hr == S_OK);
	}

	return lock;
}


Handle::Status  Voice::Vol(float v)
{
	Handle::Lock lock(this->handle);

	if (lock)
	{
		HRESULT hr;
		hr = poSourceVoice->SetVolume(v);
		assert(hr == S_OK);
	}

	return lock;
}


Handle::Status Voice::Pan(float snd_pan)
{
	Handle::Lock lock(this->handle);

	if (lock)
	{
		// Get the Audio Engine... 
		IXAudio2MasteringVoice *pMasterVoice = AudioEngine::GetMasterVoice();

		assert(pMasterVoice);
		assert(this->poSourceVoice);

		DWORD dwChannelMask;
		pMasterVoice->GetChannelMask(&dwChannelMask);

		// SPEAKER_STEREO // SPEAKER_FRONT_LEFT (0x1) | SPEAKER_FRONT_RIGHT (0x2)
		assert(dwChannelMask == 0x3);

		// pan of -1.0 indicates all left speaker, 
		// 1.0 is all right speaker, 0.0 is split between left and right
		float left = 0.5f - snd_pan / 2;
		float right = 0.5f + snd_pan / 2;

		float outputMatrix[8];
		for (int i = 0; i < 8; i++)
		{
			outputMatrix[i] = 0;
		}

		outputMatrix[0] = left;
		outputMatrix[1] = right;

		// Assuming pVoice sends to pMasteringVoice
		XAUDIO2_VOICE_DETAILS VoiceDetails;
		this->poSourceVoice->GetVoiceDetails(&VoiceDetails);

		XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
		pMasterVoice->GetVoiceDetails(&MasterVoiceDetails);

		if (this->poSourceVoice->SetOutputMatrix(NULL, VoiceDetails.InputChannels, MasterVoiceDetails.InputChannels, outputMatrix) != S_OK)
		{
			assert(false);
		}
	}
	else
	{
		assert(false);
	}

	return lock;
}

// --- End of File ---
