/*
This file will be #included before anything else in the project
*/

#ifndef EAE6320_PRECOMPILED_H
#define EAE6320_PRECOMPILED_H

// Windows
//========

// Exclude extraneous Windows stuff
#define WIN32_LEAN_AND_MEAN
// Prevent Windows from creating min/max macros
#define NOMINMAX

// Initialize Windows
#include <Windows.h>
#include "UserSettings.h"
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

// User Settings
//==============

// These will eventually come from an external file


#endif	// EAE6320_PRECOMPILED_H
