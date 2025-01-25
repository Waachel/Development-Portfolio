//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "XAudio2Wrapper.h"
#include "AudioEngine.h"
#include "Wave.h"
#include "StringThis.h"
#include "Audio.h"
#include "Aux_I_File_CB_Cmd.h"
#include "QueueMan.h"
#include "UserAsyncLoadCallback.h"
#include "Game_TransferAsyncLoadUserCallback_Cmd.h"

Wave::Wave()
	: poWfx(nullptr),
	poRawBuff(nullptr),
	rawBuffSize(0),
	id(ID::Empty),
	strName("empty"),
	pIFileCB(nullptr),
	pUserAsyncLoadCallback(nullptr),
	status(Status::EMPTY),
	handle()
{
	// nothing dynamic...

	// Pattern:
	//    Manager calls default wave
	//    set(...)  dynamic allocation
	//    privClear(...) release dynamic allocation
	//    destructor release of any owned objects
}

Wave::~Wave()
{
	//Debug::out("~Wave()\n");
	delete this->poWfx;
	delete[] this->poRawBuff;
}

void Wave::SetId(Wave::ID _id)
{
	this->id = _id;
}

Wave::ID Wave::GetId() const
{
	return this->id;
}

void Wave::SetPending(const char *const pWaveName, Wave::ID wave_id, I_File_CB *_pIFileCB)
{
	assert(pWaveName);
	assert(_pIFileCB);

	// Sets rawBuffSize, new poWfx, new poRawBuff
	this->id = wave_id;
	this->privSetName(pWaveName);
	this->pIFileCB = _pIFileCB;
	this->pUserAsyncLoadCallback = nullptr;
	this->status = Wave::Status::PENDING;
}

void Wave::SetPending(const char* const pWaveName, Wave::ID wave_id, UserAsyncLoadCallback* _pUserAsyncLoadCallback)
{
	assert(pWaveName);
	assert(_pUserAsyncLoadCallback);

	// Sets rawBuffSize, new poWfx, new poRawBuff
	this->id = wave_id;
	this->privSetName(pWaveName);
	this->pIFileCB = nullptr;
	this->pUserAsyncLoadCallback = _pUserAsyncLoadCallback;
	this->status = Wave::Status::PENDING;
}

void Wave::Dump()
{
	// Dump - Print contents to the debug output window
	Trace::out("\t\tWave(%p) %s \"%s\" \n", this, StringMe(this->id), this->strName);
}

void Wave::privClear()
{
	// This method... is used in wash to reuse the wave
	// Reset the fmt
	delete this->poWfx;
	this->poWfx = nullptr;

	delete[] this->poRawBuff;
	this->poRawBuff = nullptr;

	this->rawBuffSize = 0;

	// ID
	this->id = Wave::ID::Empty;

	// Name
	char *pWaveName = "Empty";
	this->privSetName(pWaveName);

	//if(this->pIFileCB)
	//{
	//	delete this->pIFileCB;
	//}
	this->pIFileCB = nullptr;

	this->status = Status::EMPTY;
}

void Wave::Wash()
{
	// Wash - clear the entire hierarchy
	DLink::Clear();

	// Sub class clear
	this->privClear();
}

bool Wave::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	Wave *pDataB = (Wave *) pTarget;

	bool s = false;

	if(this->id == pDataB->id)
	{
		s = true;
	}

	return s;
}

void Wave::privSetName(const char *const pWaveName)
{
	assert(pWaveName);
	memset(this->strName, 0x0, Wave::NAME_SIZE);

	// Quick hack to trim the string before '/
	unsigned int len = strlen(pWaveName);

	const char *pEnd = pWaveName + len;
	const char *pStart = pEnd;

	while(pStart != pWaveName)
	{
		if(*pStart == '/')
		{
			pStart++;
			break;
		}
		pStart--;
	}

	// At this point we have a trimmed name
	strcpy_s(this->strName, Wave::NAME_SIZE, pStart);
}

void Wave::Register(WAVEFORMATEXTENSIBLE *_poWfx, RawData *_poRawBuff, unsigned long _rawBuffSize)
{
	//Debug::out("Wave::Register()\n");

	assert(_poWfx);
	this->poWfx = _poWfx;

	assert(_poRawBuff);
	this->poRawBuff = _poRawBuff;

	assert(_rawBuffSize > 0);
	this->rawBuffSize = _rawBuffSize;

	this->status = Wave::Status::READY;

	WaveTable *pWaveTable = Audio::GetWaveTable();
	assert(pWaveTable);
	pWaveTable->Update(this->id, Wave::Status::READY);

	// Blocking callback
	// this->pIFileCB->Execute();

	Aux_I_File_CB_Cmd *pCmd = new Aux_I_File_CB_Cmd(this->pIFileCB);

//	Debug::out("--> Aux_I_File_CB_Cmd \n");
	bool s = QueueMan::SendAux(pCmd);
	assert(s);

}


void Wave::AsyncRegister(WAVEFORMATEXTENSIBLE* _poWfx, RawData* _poRawBuff, unsigned long _rawBuffSize)
{
	//Debug::out("Wave::Register()\n");

	assert(_poWfx);
	this->poWfx = _poWfx;

	assert(_poRawBuff);
	this->poRawBuff = _poRawBuff;

	assert(_rawBuffSize > 0);
	this->rawBuffSize = _rawBuffSize;

	this->status = Wave::Status::READY;

	WaveTable* pWaveTable = Audio::GetWaveTable();
	assert(pWaveTable);
	pWaveTable->Update(this->id, Wave::Status::READY);

	// Async callback
	assert(this->pUserAsyncLoadCallback);
	Game_TransferAsyncLoadUserCallback_Cmd* pCmd = new Game_TransferAsyncLoadUserCallback_Cmd(this->pUserAsyncLoadCallback);
	assert(pCmd);

	//Debug::out("--> Game_TransferUserCallback_Cmd \n");
	QueueMan::SendGame(pCmd);

}


void Wave::privLoadBuffer(const char *const pWaveName)
{
	assert(pWaveName);

	//--------------------------------------------------------
	// zero out wfx and the buffer
	//--------------------------------------------------------

	this->poWfx = new WAVEFORMATEXTENSIBLE();
	assert(this->poWfx);

	*this->poWfx = { { 0 } };    // zeros out the complete structure

	// -------------------------------------------------------
	// Open File
	// -------------------------------------------------------

	FileSlow::Handle FileHandle;
	FileSlow::Error ferror;

	// Open file

	// Take the name and add a path to it.
	const char *pDir = "..//..//AudioFiles//";
	size_t size = strlen(pDir) + strlen(pWaveName) + 1;
	char *pFullNameAndPath = new char[size]();
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
	if(filetype != fourccWAVE)
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
}

// --- End of File ---