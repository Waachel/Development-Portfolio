//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef I_FILE_CB_H
#define I_FILE_CB_H

class I_File_CB
{
public:
	I_File_CB(bool &DoneFlag);
	I_File_CB(const I_File_CB &) = delete;
	I_File_CB &operator = (const I_File_CB &) = delete;
	~I_File_CB() = default;

	void Execute();

private:
	bool &rDoneFlag;
};

#endif

//---  End of File ---