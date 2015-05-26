/*
This file defines the interface for building assets
*/

#ifndef EAE6320_ASSETBUILDER_H
#define EAE6320_ASSETBUILDER_H

// Header Files
//=============

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include <cstdint>
#include <string>

// Initialize Lua, Build assets using AssetBuilder.lua, Shutdown lua


bool BuildAssets(const char* i_relativePath);

// Initialization / Shut Down
//---------------------------

bool Initialize();
bool ShutDown();

#endif	// EAE6320_ASSETBUILDER_H
