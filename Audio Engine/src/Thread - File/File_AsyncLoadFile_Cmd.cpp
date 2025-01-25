#include "File_AsyncLoadFile_Cmd.h"
#include "Audio_AsyncFileLoadCompleted_Cmd.h"
#include "QueueMan.h"

using namespace ThreadFramework;

File_AsyncLoadFile_Cmd::File_AsyncLoadFile_Cmd(Wave::ID id, const char* const _pWaveName, Wave* _pWave)
	: Command(),
	wave_id(id),
	pWaveName(_pWaveName),
	pWave(_pWave)
{
	assert(pWaveName);
	assert(pWave);
}

File_AsyncLoadFile_Cmd::~File_AsyncLoadFile_Cmd()
{

}

// From Audio --> File to execute
void File_AsyncLoadFile_Cmd::Execute()
{
	Debug::out("File_AsyncLoadFile_Cmd::Execute(%s)\n", this->pWaveName);

	// Load the file and fill in the data structure
	// This will block on File Thread doing the loading.
	Audio_AsyncFileLoadCompleted_Cmd* pCmd = new Audio_AsyncFileLoadCompleted_Cmd(this->pWaveName, this->pWave);
	assert(pCmd);

	// Send the data to the Audio thread to register
//	Debug::out("--> Audio_AsyncFileLoadCompleted_Cmd \n");
	bool status = QueueMan::SendAudio(pCmd);
	assert(status == true);

	delete this;
}


// --- End of File ---
