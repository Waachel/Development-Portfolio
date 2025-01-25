//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "XAudio2Chunk.h"
#include "XAudio2Endianess.h"

// ----------------------------------------------------------------
// Find wave audio data chunk
// ----------------------------------------------------------------

ChunkError FindChunk(FileSlow::Handle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwOffset = 0;
	
	// Set file pointer to the beginning
	if( FileSlow::Error::SUCCESS != FileSlow::Seek(hFile, FileSlow::Location::BEGIN, 0) )
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// loop until you find it
	while (cerror == ChunkError::CHUNK_SUCCESS)
	{
		// Read data Type
		if (FileSlow::Error::SUCCESS != FileSlow::Read(hFile, &dwChunkType, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		// Read data size
		if (FileSlow::Error::SUCCESS != FileSlow::Read(hFile, &dwChunkDataSize, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			// Read data size, special format for RIFF header
			if (FileSlow::Error::SUCCESS != FileSlow::Read(hFile, &dwFileType, sizeof(DWORD)))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
			break;

		default:
			// Move file pointer to the next Chunk, continue
			if (FileSlow::Error::SUCCESS != FileSlow::Seek(hFile, FileSlow::Location::CURRENT, (int) dwChunkDataSize))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
		}

		// Since the headers are only 2 words long... 
		// Keep a running total
		dwOffset += sizeof(DWORD) * 2;

		// Found type
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			break;
		}

		dwOffset += dwChunkDataSize;

	}  // while()

	return cerror;
}

// ---------------------------------------------------------------------
// Read wave data chunk, using our FILE system
// ---------------------------------------------------------------------

ChunkError ReadChunkData(FileSlow::Handle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;

	// Seek to the offset to chunk
	if( FileSlow::Error::SUCCESS != FileSlow::Seek(hFile, FileSlow::Location::BEGIN, (int)bufferoffset) )
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// Read data chunk 
	if (FileSlow::Error::SUCCESS != FileSlow::Read(hFile, buffer, buffersize))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	return cerror;
}

//---  End of File ---
