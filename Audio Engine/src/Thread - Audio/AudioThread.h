//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef AUDIO_THREAD_H
#define AUDIO_THREAD_H

#include "CircularData.h"

void AudioMain(std::atomic_bool &QuitFlag, std::atomic_bool &AudioReadyFlag);

#endif

//---  End of File ---

