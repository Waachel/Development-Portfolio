//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "VoiceCallback.h"

VoiceCallback::~VoiceCallback()
{
	//Debug::out("~VoiceCallback(%p)\n", this);
}

VoiceCallback::VoiceCallback()
	: type(VoiceCallback::Type::Uninitialized),
	status(VoiceCallback::Status::Uninitialzed),
	pASnd(nullptr),
	pVoice(nullptr)
{

}

VoiceCallback::VoiceCallback(const VoiceCallback &r)
	:type(r.type),
	status(VoiceCallback::Status::Uninitialzed),
	pASnd(nullptr),
	pVoice(nullptr)
{

}

VoiceCallback::VoiceCallback(Type t)
	: type(t),
	status(VoiceCallback::Status::Uninitialzed),
	pASnd(nullptr),
	pVoice(nullptr)
{

}

void VoiceCallback::SetVoice(Voice *_pVoice)
{
	assert(_pVoice);
	this->pVoice = _pVoice;
}

void VoiceCallback::SetASnd(ASnd *_pASnd)
{
	this->pASnd = _pASnd;
}

// --- End of File ---
