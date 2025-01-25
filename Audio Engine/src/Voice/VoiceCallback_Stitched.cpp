//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "VoiceCallback_Stitched.h"
#include "WaveMan.h"
#include "XAudio2Wrapper.h"
#include "Voice.h"

VoiceCallback_Stitched::VoiceCallback_Stitched()
	: VoiceCallback(Type::Stitched),
	count(0),
	finished(false),
	wave{ Wave::ID::Empty }
{
}

VoiceCallback_Stitched::VoiceCallback_Stitched(Wave::ID wA, Wave::ID wB, Wave::ID wC)
	: VoiceCallback(Type::Stitched),
	count(0),
	finished(false),
	wave{ Wave::ID::Empty }
{
	this->wave[0] = wA;
	this->wave[1] = wB;
	this->wave[2] = wC;
}

VoiceCallback_Stitched::VoiceCallback_Stitched(Wave::ID wA, Wave::ID wB, Wave::ID wC, Wave::ID wD, Wave::ID wE, Wave::ID wF, Wave::ID wG)
	: VoiceCallback(Type::Stitched),
	count(0),
	finished(false),
	wave{ Wave::ID::Empty }
{
	this->wave[0] = wA;
	this->wave[1] = wB;
	this->wave[2] = wC;
	this->wave[3] = wD;
	this->wave[4] = wE;
	this->wave[5] = wF;
	this->wave[6] = wG;
}

VoiceCallback_Stitched::VoiceCallback_Stitched(const VoiceCallback_Stitched& r)
	: VoiceCallback(r.type),
	count(r.count),
	finished(r.finished),
	wave{ Wave::ID::Empty }
{
	wave[0] = r.wave[0];
	wave[1] = r.wave[1];
	wave[2] = r.wave[2];
	wave[3] = r.wave[3];
	wave[4] = r.wave[4];
	wave[5] = r.wave[5];
	wave[6] = r.wave[6];

	assert(r.type == Type::Stitched);
	assert(r.count == 0);
	assert(r.finished == false);
	// pVoice should be null its externally set
	assert(this->pVoice == nullptr);

	assert(this->wave[0] == r.wave[0]);
	assert(this->wave[1] == r.wave[1]);
	assert(this->wave[2] == r.wave[2]);
	assert(this->wave[3] == r.wave[3]);
	assert(this->wave[4] == r.wave[4]);
	assert(this->wave[5] == r.wave[5]);
	assert(this->wave[6] == r.wave[6]);
}

VoiceCallback *VoiceCallback_Stitched::CreateCallbackCopy()
{
	return new VoiceCallback_Stitched(*this);
}


VoiceCallback_Stitched::~VoiceCallback_Stitched()
{
	//Debug::out("~VoiceCallback_Stitched(%p) \n", this);
}

void VoiceCallback_Stitched::OnStreamEnd()
{
	Debug::out("VoiceCallback_Stitched::StreamEnd()\n");
	finished = true;
}

void VoiceCallback_Stitched::OnVoiceProcessingPassEnd()
{
}

void VoiceCallback_Stitched::OnVoiceProcessingPassStart(UINT32)
{

}

void VoiceCallback_Stitched::OnBufferEnd(void*)
{
	//Debug::out("VoiceCallback_Stitched::OnBufferEnd()\n");

	if (count < 7)
	{
		XAUDIO2_BUFFER_ALIAS* pBuffer = this->pVoice->poBuff;
		assert(pBuffer);

		Wave* pWave = WaveMan::Find(wave[count]);
		assert(pWave);
		Debug::out("Wav: %s \n", pWave->strName);

		pBuffer->AudioBytes = pWave->rawBuffSize;
		pBuffer->pAudioData = pWave->poRawBuff;

		if (count == 6)
		{
			pBuffer->Flags = XAUDIO2_END_OF_STREAM;
		}

		HRESULT hr;

		if (FAILED(hr = this->pVoice->poSourceVoice->SubmitSourceBuffer((XAUDIO2_BUFFER*)pBuffer)))
		{
			assert(false);
		}
		count++;
	}
}
void VoiceCallback_Stitched::OnBufferStart(void *)
{
}

void VoiceCallback_Stitched::OnLoopEnd(void *)
{
}

void VoiceCallback_Stitched::OnVoiceError(void *, HRESULT)
{
}

// --- End of File ---