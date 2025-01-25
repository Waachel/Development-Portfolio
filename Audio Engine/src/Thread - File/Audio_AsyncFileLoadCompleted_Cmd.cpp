
#include "Audio_AsyncFileLoadCompleted_Cmd.h"

using namespace ThreadFramework;

Audio_AsyncFileLoadCompleted_Cmd::Audio_AsyncFileLoadCompleted_Cmd(const char* const pWaveName, Wave* _pWave)
	: poWfx(nullptr),
	poRawBuff(nullptr),
	rawBuffSize(0),
	pWave(_pWave)
{
	assert(pWave);
	this->LoadBuffer(pWaveName);
}

void Audio_AsyncFileLoadCompleted_Cmd::Execute()
{
	Debug::out("Audio_AsyncFileLoadCompleted_Cmd::Execute()\n");

	assert(this->pWave);
	this->pWave->AsyncRegister(poWfx, poRawBuff, rawBuffSize);

	delete this;
}

void Audio_AsyncFileLoadCompleted_Cmd::LoadBuffer(const char* const pWaveName)
{
	assert(pWaveName);

	//Debug::out("Audio_AsyncFileLoadCompleted_Cmd::LoadBuffer(%s) start\n", pWaveName);

	//--------------------------------------------------------
	// zero out wfx and the buffer
	//--------------------------------------------------------

	this->poWfx = new WAVEFORMATEXTENSIBLE();
	assert(this->poWfx);

	*this->poWfx = { {0} };    // zeros out the complete structure

	// -------------------------------------------------------
	// Open File
	// -------------------------------------------------------

	FileSlow::Handle FileHandle;
	FileSlow::Error ferror;

	// Open file
		// Take the name and add a path to it.
	const char* pDir = "..//..//AudioFiles//";
	size_t size = strlen(pDir) + strlen(pWaveName) + 1;
	char* pFullNameAndPath = new char[size]();
	strcat_s(pFullNameAndPath, size, pDir);
	strcat_s(pFullNameAndPath, size, pWaveName);

	ferror = FileSlow::Open(FileHandle, pFullNameAndPath, FileSlow::Mode::READ);
	assert(ferror == FileSlow::Error::SUCCESS);

	delete[] pFullNameAndPath;

	// Set file to beginning
	ferror = FileSlow::Seek(FileHandle, FileSlow::Location::BEGIN, 0);
	assert(ferror == FileSlow::Error::SUCCESS);

	// -------------------------------------------------------
	// Find and load specific Chunks
	// -------------------------------------------------------

	DWORD dwChunkPosition;
	DWORD filetype;
	ChunkError cerror;

	// Scan to the RIFF and load file type
	cerror = FindChunk(FileHandle, fourccRIFF, this->rawBuffSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);
	cerror = ReadChunkData(FileHandle, &filetype, sizeof(DWORD), dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	// Make sure its not in the compressed format, WAVE format is uncompressed
	if (filetype != fourccWAVE)
	{
		assert(false);
	}

	// Read the FMT: format
	cerror = FindChunk(FileHandle, fourccFMT, this->rawBuffSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);
	cerror = ReadChunkData(FileHandle, this->poWfx, this->rawBuffSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	// Scan to the DATA chunk, read the size, allocate buffer of that size
	cerror = FindChunk(FileHandle, fourccDATA, this->rawBuffSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	this->poRawBuff = new unsigned char[this->rawBuffSize];
	assert(this->poRawBuff);

	// Fill the data...
	cerror = ReadChunkData(FileHandle, this->poRawBuff, this->rawBuffSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	ferror = FileSlow::Close(FileHandle);
	assert(ferror == FileSlow::Error::SUCCESS);

	Debug::out("Audio_AsyncFileLoadCompleted_Cmd::LoadBuffer(%s) end\n", pWaveName);
}
