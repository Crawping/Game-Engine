// Header Files
//=============

#include "HelperFunctions.h"

#include <iostream>
#include <ShlObj.h>
#include <sstream>

// Static Data Initialization
//===========================

namespace
{
	std::string s_AuthoredAssetDir;
	std::string s_BuiltAssetDir;
}

// Function Definitions
//=====================

void OutputErrorMessage(const char* i_errorMessage, const char* i_optionalFileName)
{
	std::cerr << (i_optionalFileName ? i_optionalFileName : "Asset Build") << ": error: " <<
		i_errorMessage << "\n";
}

// Windows Functions
//------------------

bool CopyFile(const char* i_path_source, const char* i_path_target)
{
	const BOOL shouldFailIfTargetFileAlreadyExists = FALSE;
	if (CopyFile(i_path_source, i_path_target, shouldFailIfTargetFileAlreadyExists) != FALSE)
	{
		return true;
	}
	else
	{
		OutputErrorMessage(GetLastWindowsError().c_str(), i_path_source);
		return false;
	}
}



bool GetEnvironmentVariable(const char* i_key, std::string& o_value)
{
	// Windows requires a character buffer
	// to copy the environment variable into.
	// An arbitrary value is chosen that "should" be "big enough":
	const DWORD maxCharacterCount = 512;
	char buffer[maxCharacterCount];
	// Ask Windows for the environment variable
	const DWORD characterCount = GetEnvironmentVariable(i_key, buffer, maxCharacterCount);
	if (characterCount > 0)
	{
		if (characterCount <= maxCharacterCount)
		{
			o_value = buffer;
			return true;
		}
		else
		{
			// If you're seeing this error you will need to increase maxCharacterCount
			std::stringstream errorMessage;
			errorMessage << "The environment variable \"" << i_key << "\" requires " << characterCount <<
				" characters and the provided buffer only has room for " << maxCharacterCount;
			OutputErrorMessage(errorMessage.str().c_str());
			return false;
		}
	}
	else
	{
		DWORD errorCode;
		std::string errorString = GetLastWindowsError(&errorCode);
		if (errorCode == ERROR_ENVVAR_NOT_FOUND)
		{
			// If you're seeing this error and the environment variable is spelled correctly
			// it _probably_ means that you are debugging and haven't set up the environment.
			//	* Right click the project name and choose "Properties"
			//	* In the tree view to the left select "Configuration Properties->Debugging"
			//	* In the "Environment" field make sure that you have this environment variable set like:
			//		* someKey=$(someKey)
			std::string errorMessage = std::string("The environment variable \"") +
				i_key + "\" doesn't exist";
			OutputErrorMessage(errorMessage.c_str());
		}
		else
		{
			std::string errorMessage = std::string("Windows failed to get the environment variable \"") +
				i_key + "\": " + errorString;
			OutputErrorMessage(errorMessage.c_str());
		}
		return false;
	}
}

std::string GetFormattedWindowsError(const DWORD i_errorCode)
{
	std::string errorMessage;
	{
		const DWORD formattingOptions =
			// Get the error message from Windows
			FORMAT_MESSAGE_FROM_SYSTEM
			// The space for the error message should be allocated by Windows
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			// Any potentially deferred inserts should be ignored
			// (i.e. the error message should be in its final form)
			| FORMAT_MESSAGE_IGNORE_INSERTS;
		const void* messageIsFromWindows = NULL;
		const DWORD useTheDefaultLanguage = 0;
		char* messageBuffer = NULL;
		const DWORD minimumCharacterCountToAllocate = 1;
		va_list* insertsAreIgnored = NULL;
		const DWORD storedCharacterCount = FormatMessage(formattingOptions, messageIsFromWindows, i_errorCode,
			useTheDefaultLanguage, reinterpret_cast<LPSTR>(&messageBuffer), minimumCharacterCountToAllocate, insertsAreIgnored);
		if (storedCharacterCount != 0)
		{
			errorMessage = messageBuffer;
		}
		else
		{
			// If there's an error GetLastError() can be called again,
			// but that is too complicated for this program :)
			errorMessage = "Unknown Windows Error";
		}
		// Try to free the memory regardless of whether formatting worked or not,
		// and ignore any error messages
		LocalFree(messageBuffer);
	}
	return errorMessage;
}

std::string GetLastWindowsError(DWORD* o_optionalErrorCode)
{
	// Windows stores the error as a code
	const DWORD errorCode = GetLastError();
	if (o_optionalErrorCode)
	{
		*o_optionalErrorCode = errorCode;
	}
	return GetFormattedWindowsError(errorCode);
}

