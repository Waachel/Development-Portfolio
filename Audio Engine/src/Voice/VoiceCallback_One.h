//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef VOICE_CALLBACK_ONE_H
#define VOICE_CALLBACK_ONE_H

#include "XAudio2Wrapper.h"
#include "VoiceCallback.h"

class Voice;

using namespace ThreadFramework;

//  Sample voice callback
class VoiceCallback_One : public VoiceCallback
{
public:
	bool   finished;

	VoiceCallback_One();
	VoiceCallback_One(const VoiceCallback_One &);
	VoiceCallback_One(VoiceCallback_One &&) = delete;
	VoiceCallback_One &operator = (const VoiceCallback_One &) = delete;
	VoiceCallback_One &operator = (VoiceCallback_One &&) = delete;
	virtual ~VoiceCallback_One();

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