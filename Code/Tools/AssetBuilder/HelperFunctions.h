/*
This example program has the helper funtions in a separate file
to make it easier for you to keep track of program flow
*/

#ifndef EAE6320_HELPERFUNCTIONS_H
#define EAE6320_HELPERFUNCTIONS_H

// Header Files
//=============

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include <cstdint>
#include <string>
#include "../../External/Lua/Includes.h"

// Function Declarations
//======================

// Errors can be formatted a specific way so that they show up
// in Visual Studio's "Error List" tab
void OutputErrorMessage(const char* i_errorMessage, const char* i_optionalFileName = NULL);


// Helper Functions
bool CopyFile(const char* i_path_source, const char* i_path_target);
bool GetEnvironmentVariable(const char* i_key, std::string& o_value);
std::string GetFormattedWindowsError(const DWORD i_errorCode);
std::string GetLastWindowsError(DWORD* o_optionalErrorCode = NULL);

#endif	// EAE6320_HELPERFUNCTIONS_H
