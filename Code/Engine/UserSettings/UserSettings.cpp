// Header Files
//=============
#include "Windows.h"
#include "FileAPI.h"
#include "UserSettings.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "../../External/Lua/Includes.h"

// Static Data Initialization
//===========================

namespace
{
	unsigned int s_height = 480;
	bool s_isFullScreenModeEnabled = false;
	unsigned int s_width = 640;

	const char* s_userSettingsfileName = "settings.ini";
}

// Helper Function Declarations
//=============================

namespace
{
	bool InitializeIfNecessary();
	bool IsNumberAnInteger( const lua_Number i_number );
	bool LoadUserSettingsIntoLuaTable( lua_State& io_luaState, bool& o_doesUserSettingsFileExist );
	bool PopulateUserSettingsFromLuaTable( lua_State& io_luaState );
}

// Interface
//==========

unsigned int eae6320::UserSettings::GetHeight()
{
	InitializeIfNecessary();
	return s_height;
}

unsigned int eae6320::UserSettings::GetWidth()
{
	InitializeIfNecessary();
	return s_width;
}

unsigned int eae6320::UserSettings::IsFullScreenModeEnabled()
{
	InitializeIfNecessary();
	return s_isFullScreenModeEnabled;
}

// Helper Function Definitions
//============================

namespace
{
	bool InitializeIfNecessary()
	{
		static bool isInitialized = false;
		if ( isInitialized )
		{
			return true;
		}

		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		bool wasUserSettingsEnvironmentCreated = false;
		{
			luaState = luaL_newstate();
			if ( luaState == NULL )
			{
				MessageBox( NULL, "Failed to create a new Lua state", "No User Settings", MB_OK | MB_ICONERROR );
				wereThereErrors = true;
				goto CleanUp;
			}
		}
		// Create an empty table to be used as the Lua environment for the user settings
		{
			lua_newtable( luaState );
			wasUserSettingsEnvironmentCreated = true;
		}
		// Load the user settings
		bool doesUserSettingsFileExist;
		if ( LoadUserSettingsIntoLuaTable( *luaState, doesUserSettingsFileExist ) )
		{
			if ( doesUserSettingsFileExist )
			{
				// Populate the user settings in C from the user settings in the Lua environment
				if ( !PopulateUserSettingsFromLuaTable( *luaState ) )
				{
					wereThereErrors = true;
					goto CleanUp;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			goto CleanUp;
		}

		// Free the Lua environment

	CleanUp:

		if ( luaState )
		{
			if ( wasUserSettingsEnvironmentCreated )
			{
				lua_pop( luaState, 1 );
			}
			// Sanity Check
			{
				int stackItemCount = lua_gettop( luaState );
				if ( stackItemCount != 0 )
				{
					std::ostringstream errorMessage;
					errorMessage << "There are still " << stackItemCount
						<< " items in the Lua stack when the Lua state is being closed";
					MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
					wereThereErrors = true;
				}
			}
			lua_close( luaState );
		}

		isInitialized = !wereThereErrors;
		return isInitialized;
	}

	bool IsNumberAnInteger( const lua_Number i_number )
	{
		lua_Number integralPart;
		lua_Number fractionalPart = modf( i_number, &integralPart );
		return integralPart == i_number;
	}

	bool LoadLuaNumber(lua_State& io_luaState, const char* i_key, int& stackValues)
	{
		const char* key = i_key;
		lua_pushstring(&io_luaState, key);
		stackValues++;
		{
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);
		}

		// Make sure value exists in table
		if (lua_isnil(&io_luaState, -1))
		{
			std::cerr << "No value for \"" << key << "\" was found in the asset table\n";
			lua_pop(&io_luaState, stackValues);
			stackValues = 0;
			return false;
		}

		// Make sure value is a number
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
		{
			std::cerr << "The value for \"" << key << "\" must be a number "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
			// Pop the values
			lua_pop(&io_luaState, stackValues);
			stackValues = 0;
			// (The asset table is now at -1)
			return false;
		}
		return true;
	}

	bool LoadLuaBoolean(lua_State& io_luaState, const char* i_key, int& stackValues)
	{
		const char* key = i_key;
		lua_pushstring(&io_luaState, key);
		stackValues++;
		{
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);
		}

		// Make sure value exists in table
		if (lua_isnil(&io_luaState, -1))
		{
			std::cerr << "No value for \"" << key << "\" was found in the asset table\n";
			lua_pop(&io_luaState, stackValues);
			stackValues = 0;
			return false;
		}

		// Make sure value is a Boolean
		if (lua_type(&io_luaState, -1) != LUA_TBOOLEAN)
		{
			std::cerr << "The value for \"" << key << "\" must be a boolean "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
			// Pop the values
			lua_pop(&io_luaState, stackValues);
			stackValues = 0;
			// (The asset table is now at -1)
			return false;
		}
		return true;
	}

	bool LoadUserSettingsIntoLuaTable( lua_State& io_luaState, bool& o_doesUserSettingsFileExist )
	{
		// Load the file into a Lua function
		int result = luaL_loadfile( &io_luaState, s_userSettingsfileName );
		if ( result == LUA_OK )
		{
			o_doesUserSettingsFileExist = true;

			// Set the Lua function's environment
			{
				lua_pushvalue( &io_luaState, -2 );
				const char* upValueName = lua_setupvalue( &io_luaState, -2, 1 );
				if ( upValueName == NULL )
				{
					std::stringstream errorMessage;
					errorMessage << "Internal error setting the Lua environment for " << s_userSettingsfileName << "... This should never happen";
					MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
					lua_pop( &io_luaState, 2 );
					return false;
				}
			}
			// Call the Lua function
			{
				int noArguments = 0;
				int noReturnValues = 0;
				int noErrorMessageHandler = 0;
				result = lua_pcall( &io_luaState, noArguments, noReturnValues, noErrorMessageHandler );
				if ( result == LUA_OK )
				{
					return true;
				}
				else
				{
					std::string luaErrorMessage( lua_tostring( &io_luaState, -1 ) );
					lua_pop( &io_luaState, 1 );

					if ( result == LUA_ERRRUN )
					{
						std::stringstream errorMessage;
						errorMessage << "Error in " << s_userSettingsfileName << ": " << luaErrorMessage;
						MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "Error processing user settings file: " << luaErrorMessage;
						MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
					}

					return false;
				}
			}
		}
		else
		{
			o_doesUserSettingsFileExist = false;

			std::string luaErrorMessage( lua_tostring( &io_luaState, -1 ) );
			lua_pop( &io_luaState, 1 );

			if ( result == LUA_ERRFILE )
			{
				// If loading the file failed because the file doesn't exist it's ok
				if ( GetFileAttributes( s_userSettingsfileName ) == INVALID_FILE_ATTRIBUTES )
				{
					return true;
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "User settings file " << s_userSettingsfileName <<
						" exists but couldn't be opened or read: " << luaErrorMessage;
					MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
				}
			}
			else if ( result == LUA_ERRSYNTAX )
			{
				std::stringstream errorMessage;
				errorMessage << "Syntax error in " << s_userSettingsfileName << ": " << luaErrorMessage;
				MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "Error loading user settings file: " << luaErrorMessage;
				MessageBox( NULL, errorMessage.str().c_str(), "No User Settings", MB_OK | MB_ICONERROR );
			}

			return false;
		}
	}

	bool IsThisWidthAndHeightSupported(int iWidth, int iHeight)
	{
		DEVMODE dm = { 0 };
		dm.dmSize = sizeof(dm);

		for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
		{
			if (dm.dmPelsWidth == iWidth && dm.dmPelsHeight == iHeight)
				return true;
		}

		return false;
	}

	bool PopulateUserSettingsFromLuaTable( lua_State& io_luaState )
	{
		int stackvalues = 0;

		// load width
		if (!LoadLuaNumber(io_luaState, "width", stackvalues))
		{
			s_width = 800;
		}
		else
		{
			s_width = lua_tointeger(&io_luaState, -1);
			lua_pop(&io_luaState, 1);
			stackvalues--;
		}


		// load height
		if (!LoadLuaNumber(io_luaState, "height", stackvalues))
		{
			s_height = 600;
		}
		else
		{
			s_height = lua_tointeger(&io_luaState, -1);
			lua_pop(&io_luaState, 1);
			stackvalues--;
		}

		// load fullscreen
		if (!LoadLuaBoolean(io_luaState, "fullscreen", stackvalues))
		{
			s_isFullScreenModeEnabled = false;
		}
		else
		{
			s_isFullScreenModeEnabled = lua_toboolean(&io_luaState, -1) ? true:false;
			lua_pop(&io_luaState, 1);
			stackvalues--;

			if (s_width != 800 || s_height != 600)
			{
				/*if (!IsThisWidthAndHeightSupported(s_width, s_height))
				{
					MessageBox(NULL, "Resolution not supported", "Invalid Resolution",MB_OK|MB_ICONERROR);
					s_width = 800;
					s_height = 600;
				}*/

				if (s_width > 1920)
				{
					s_width = 1920;
				}
				if (s_height > 1080)
				{
					s_height = 1080;
				}
			}
		}

		return true;
	}
}

