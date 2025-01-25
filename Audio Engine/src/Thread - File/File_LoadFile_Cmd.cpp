#include "File_LoadFile_Cmd.h"
#include "Audio_FileLoadCompleted_Cmd.h"
#include "QueueMan.h"

using namespace ThreadFramework;

File_LoadFile_Cmd::File_LoadFile_Cmd(Wave::ID id, const char *const _pWaveName, Wave *_pWave)
	: Command(),
	wave_id(id),
	pWaveName(_pWaveName),
	pWave(_pWave)
{
	assert(pWaveName);
	assert(pWave);
}

File_LoadFile_Cmd::~File_LoadFile_Cmd()
{

}

// From Audio --> File to execute
void File_LoadFile_Cmd::Execute()
{
	Debug::out("File_LoadFile_Cmd::Execute(%s)\n", this->pWaveName);

	// Load the file and fill in the data structure
	// This will block on File Thread doing the loading.
	Audio_FileLoadCompleted_Cmd *pCmd = new Audio_FileLoadCompleted_Cmd(this->pWaveName, this->pWave);
	assert(pCmd);

	// Send the data to the Audio thread to register
//	Debug::out("--> Audio_FileLoadCompleted_Cmd \n");
	bool status = QueueMan::SendAudio(pCmd);
	assert(status == true);

	delete this;
}


// --- End of File ---