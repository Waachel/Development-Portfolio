//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef VOICE_CALLBACK_STICHED_H
#define VOICE_CALLBACK_STICHED_H

#include "XAudio2Wrapper.h"
#include "VoiceCallback.h"

#include "Wave.h"

class Voice;

using namespace ThreadFramework;

//  Sample voice callback
class VoiceCallback_Stitched : public VoiceCallback
{
public:
	int count;
	bool   finished = false;
	Wave::ID wave[8];

	VoiceCallback_Stitched();
	VoiceCallback_Stitched(const VoiceCallback_Stitched &);
	VoiceCallback_Stitched(VoiceCallback_Stitched &&) = delete;
	VoiceCallback_Stitched &operator = (const VoiceCallback_Stitched &) = delete;
	VoiceCallback_Stitched &operator = (VoiceCallback_Stitched &&) = delete;
	virtual ~VoiceCallback_Stitched();

	VoiceCallback_Stitched(Wave::ID wA, Wave::ID wB, Wave::ID wC);
	VoiceCallback_Stitched(Wave::ID wA, Wave::ID wB, Wave::ID wC, Wave::ID wD, Wave::ID wE, Wave::ID wF, Wave::ID wG);
	virtual VoiceCallback *CreateCallbackCopy() override;

	virtual void STDMETHODCALLTYPE OnStreamEnd() override;
	virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override;
	virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) override;
	virtual void STDMETHODCALLTYPE OnBufferEnd(void *) override;
	virtual void STDMETHODCALLTYPE OnBufferStart(void *) override;
	virtual void STDMETHODCALLTYPE OnLoopEnd(void *) override;
	virtual void STDMETHODCALLTYPE OnVoiceError(void *, HRESULT) override;
};

#endif

// --- End of File ---