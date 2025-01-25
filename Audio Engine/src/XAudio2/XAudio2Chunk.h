//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef XAUDIO2_CHUNK_H
#define XAUDIO2_CHUNK_H

#include "FileSlow.h"

enum ChunkError
{
	CHUNK_SUCCESS = 0xC0000000,
	CHUNK_FAIL,

};

ChunkError FindChunk(FileSlow::Handle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
ChunkError ReadChunkData(FileSlow::Handle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);

#endif

//---  End of File ---
