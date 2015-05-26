/*
	This file contains useful utility functions for other build processes
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
	#include <Windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include <cstdint>
#include <string>
#include "../../External/Lua/Includes.h"

namespace eae6320
{
	// Interface
	//==========

	bool CopyFile( const char* i_path_source, const char* i_path_target,
		const bool i_shouldFunctionFailIfTargetAlreadyExists = false, std::string* o_errorMessage = NULL );
	bool CreateDirectoryIfNecessary( const std::string& i_path, std::string* o_errorMessage = NULL );
	bool DoesFileExist( const char* i_path, std::string* o_errorMessage = NULL, DWORD* o_errorCode = NULL );
	bool GetEnvironmentVariable( const char* i_key, std::string& o_value, std::string* o_errorMessage = NULL );
	std::string GetFormattedWindowsError( const DWORD i_errorCode );
	std::string GetLastWindowsError( DWORD* o_optionalErrorCode = NULL );
	bool GetLastWriteTime( const char* i_path, uint64_t& o_lastWriteTime, std::string* o_errorMessage = NULL );
	void OutputErrorMessage( const char* i_errorMessage, const char* i_optionalFileName = NULL );
	bool ExecuteCommand(const char* i_command, DWORD* o_exitCode, std::string* o_errorMessage);
	bool ExecuteCommand(const char* i_command, const char* i_optionalArguments, DWORD* o_exitCode, std::string* o_errorMessage);
	bool LoadLuaTable(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaString(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaNumber(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaBoolean(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadFloat3WithInterval(lua_State& io_luaState, int& stackValues, unsigned int i_interval, char* io_outputBuffer);
	bool LoadInt3WithInterval(lua_State& io_luaState, int& stackValues, unsigned int i_interval, char* io_outputBuffer);
	bool LoadFloat2WithInterval(lua_State& io_luaState, int& stackValues, unsigned int i_interval, char* io_outputBuffer);
	bool LoadColorsWithInterval(lua_State& io_luaState, int& stackValues, unsigned int i_interval, char* io_outputBuffer);
	bool LoadInt3WithoutInterval(lua_State& io_luaState, int& stackValues, char* io_outputBuffer);

}
