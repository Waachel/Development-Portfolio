//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "VoiceCallback_One.h"
#include "Audio_SndEnd_Cmd.h"
#include "ASnd.h"
#include "QueueMan.h"
#include "StringThis.h"

VoiceCallback_One::~VoiceCallback_One()
{
	//Debug::out("~VoiceCallback_One(%p) \n",this);
}

VoiceCallback_One::VoiceCallback_One()
	: VoiceCallback(Type::One),
	finished(false)
{
	// pVoice should be null its externally set
	assert(this->pVoice == nullptr);
	assert(this->pASnd == nullptr);
}

VoiceCallback_One::VoiceCallback_One(const VoiceCallback_One &r)
	: VoiceCallback(r.type),
	finished(r.finished)
{
	assert(r.type == Type::One);
	assert(r.finished == false);
	// pVoice should be null its externally set
	assert(this->pVoice == nullptr);
	assert(this->pASnd == nullptr);
}

VoiceCallback * VoiceCallback_One::CreateCallbackCopy()
{
	// copy constructor
	return new VoiceCallback_One(*this);
}


void VoiceCallback_One::OnStreamEnd()
{
	finished = true;

	Snd::ID sID = this->pASnd->pSnd->snd_id;
	Debug::out("VoiceCallback: StreamEnd(%s)\n", 
		StringMe(sID));

	// Send a command and leave
	assert(this->pASnd);

	// Send a command to create ASnd
	Audio_SndEnd_Cmd *pCmd = new Audio_SndEnd_Cmd(this->pASnd->snd_id, this->pASnd->pSnd);
	assert(pCmd);

	//Debug::out("--> Audio_SndEnd_Cmd \n");
	QueueMan::SendAudio(pCmd);
}

void VoiceCallback_One::OnVoiceProcessingPassEnd()
{
}

void VoiceCallback_One::OnVoiceProcessingPassStart(UINT32)
{
	//Debug::out("processing start\n");
}

void VoiceCallback_One::OnBufferEnd(void *)
{
}

void VoiceCallback_One::OnBufferStart(void *)
{
}

void VoiceCallback_One::OnLoopEnd(void *)
{
}

void VoiceCallback_One::OnVoiceError(void *, HRESULT)
{
}

// --- End of File ---