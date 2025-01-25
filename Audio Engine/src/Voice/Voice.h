//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef VOICE_H
#define VOICE_H

#include "XAudio2Wrapper.h"
#include "Handle.h"
#include "DLink.h"
#include "Wave.h"
#include "VoiceCallback.h"

// forward declare
class AudioEngine;

struct XAUDIO2_BUFFER_ALIAS
{
	UINT32 Flags;                       // Either 0 or XAUDIO2_END_OF_STREAM.
	UINT32 AudioBytes;                  // Size of the audio data buffer in bytes.
	RawData *pAudioData;             // Pointer to the audio data buffer.
	UINT32 PlayBegin;                   // First sample in this buffer to be played.
	UINT32 PlayLength;                  // Length of the region to be played in samples,
										//  or 0 to play the whole buffer.
	UINT32 LoopBegin;                   // First sample of the region to be looped.
	UINT32 LoopLength;                  // Length of the desired loop region in samples,
										//  or 0 to loop the entire buffer.
	UINT32 LoopCount;                   // Number of times to repeat the loop region,
										//  or XAUDIO2_LOOP_INFINITE to loop forever.
	void *pContext;                     // Context value to be passed back in callbacks.
};


class Voice : public DLink
{
public:
	// Big 4
	Voice();
	Voice(const Voice &) = delete;
	Voice &operator = (const Voice &) = delete;
	virtual ~Voice();

	// Manager methods
	virtual void Wash() override;
	virtual void Dump() override;
	virtual bool Compare(DLink *pTargetNode) override;

	// Manager non-virtual

	void Set(Wave *pWave, VoiceCallback *pCallback);
	void Clear();

	// public methods:
	Handle::Status Start();
	Handle::Status Stop();
	Handle::Status Vol(float v);
	Handle::Status Pan(float pan);

private:
	void privInitSrcBuff(bool StreamEnd);

// Data
public:
	// make this private...
	Wave                  *pWave;

	XAUDIO2_BUFFER_ALIAS  *poBuff;
	VoiceCallback         *porCallback;

	IXAudio2SourceVoice   *poSourceVoice;

	Handle handle;
};

#endif

// --- End of File ---
