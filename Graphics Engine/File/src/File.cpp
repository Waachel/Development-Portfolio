//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "File.h"

namespace Azul
{

	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD flags;
		const BOOL valid = GetHandleInformation(fh, &flags);

		if (valid == 0)
		{
			return false;
		}
		//fh = INVALID_HANDLE_VALUE;
		return true;
	}

	File::Error File::Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept
	{
		DWORD modeType;

		//opening one that is already open
		bool valid = IsHandleValid(fh);
		if (valid)
		{
			return File::Error::OPEN_FAIL;
		}

		if (mode == File::Mode::WRITE)
		{
			modeType = GENERIC_WRITE;
		}
		else if (mode == File::Mode::READ)
		{
			modeType = GENERIC_READ;
		}
		//mode == File::Mode::READ_WRITE
		else
		{
			modeType = GENERIC_WRITE | GENERIC_READ;
		}
		fh = CreateFile(fileName, modeType, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);



		if (fh == INVALID_HANDLE_VALUE)
		{
			return File::Error::OPEN_FAIL;
		}

		else
		{
			return File::Error::SUCCESS;
		}

	}

	File::Error File::Close(File::Handle &fh) noexcept
	{
		//closing one that is already closed
		bool valid = IsHandleValid(fh);
		if (!valid)
		{
			return File::Error::CLOSE_FAIL;
		}


		BOOL close = CloseHandle(fh);

		if (close == 0)
		{
			return File::Error::CLOSE_FAIL;
		}
		fh = ((Handle)(LONG_PTR)0xFFFFFF);
		return File::Error::SUCCESS;
	}

	File::Error File::Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept
	{
		BOOL write = WriteFile(fh, buffer, inSize, NULL, NULL);

		if (write == 0)
		{
			return File::Error::WRITE_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Read(File::Handle fh, void* const buffer, const DWORD inSize) noexcept
	{
		BOOL read = ReadFile(fh, buffer, inSize, NULL, NULL);

		if (read == 0)
		{
			return File::Error::READ_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Seek(File::Handle fh, File::Position location, int offset) noexcept
	{
		DWORD start;
		if (location == File::Position::BEGIN)
		{
			start = FILE_BEGIN;
		}
		else if (location == File::Position::CURRENT)
		{
			start = FILE_CURRENT;
		}
		//location == File::Position::END
		else
		{
			start = FILE_END;
		}
		DWORD seek = SetFilePointer(fh, offset,NULL, start);

		if (seek == INVALID_SET_FILE_POINTER)
		{
			return File::Error::SEEK_FAIL;
		}
		return File::Error::SUCCESS;
		
	}

	File::Error File::Tell(File::Handle fh, DWORD& offset) noexcept
	{
		offset = 0;
		DWORD tell = SetFilePointer(fh, (LONG)offset, NULL, FILE_CURRENT);

		offset = (DWORD)abs((int)offset - (int)tell);
		if (tell == INVALID_SET_FILE_POINTER)
		{
			return File::Error::TELL_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Flush(File::Handle fh) noexcept
	{
		BOOL flush = FlushFileBuffers(fh);

		if (flush == 0)
		{
			return File::Error::FLUSH_FAIL;
		}
		return File::Error::SUCCESS;
	}

}

// --- End of File ---
