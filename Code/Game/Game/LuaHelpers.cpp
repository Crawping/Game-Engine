#include "precompiled.h"
#include "LuaHelpers.h"
#include <iostream>
#include "Vector3.h"
// Loads a Table value corresponding to i_key onto the top of the stack
bool LuaHelpers::LoadLuaTable(lua_State& io_luaState, const char* i_key, int& stackValues)
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
		return false;
	}

	// Make sure value is a table
	if (lua_type(&io_luaState, -1) != LUA_TTABLE)
	{
		std::cerr << "The value for \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		// Pop the value
		lua_pop(&io_luaState, stackValues);
		// (The asset table is now at -1)
		return false;
	}
	return true;

}

// Loads a String value corresponding to i_key onto the top of the stack
bool LuaHelpers::LoadLuaString(lua_State& io_luaState, const char* i_key, int& stackValues)
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
		return false;
	}

	// Make sure value is a string
	if (lua_type(&io_luaState, -1) != LUA_TSTRING)
	{
		std::cerr << "The value for \"" << key << "\" must be a string "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		// Pop the values
		lua_pop(&io_luaState, stackValues);
		// (The asset table is now at -1)
		return false;
	}
	return true;
}

bool LuaHelpers::LoadLuaNumber(lua_State& io_luaState, const char* i_key, int& stackValues)
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
		return false;
	}

	// Make sure value is a number
	if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
	{
		std::cerr << "The value for \"" << key << "\" must be a number "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		// Pop the values
		lua_pop(&io_luaState, stackValues);
		// (The asset table is now at -1)
		return false;
	}
	return true;
}
bool LuaHelpers::LoadLuaBoolean(lua_State& io_luaState, const char* i_key, int& stackValues)
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
		return false;
	}

	// Make sure value is a Boolean
	if (lua_type(&io_luaState, -1) != LUA_TBOOLEAN)
	{
		std::cerr << "The value for \"" << key << "\" must be a boolean "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		// Pop the values
		lua_pop(&io_luaState, stackValues);
		// (The asset table is now at -1)
		return false;
	}
	return true;
}

bool LuaHelpers::LoadFloat3(lua_State& io_luaState,int& stackValues, Engine::Vector3& io_vector3)
{
	float values[3];
	int currentValueCount = 0;
	lua_pushnil(&io_luaState);
	stackValues++;
	const int tableIndex = -2;
	{
		while (lua_next(&io_luaState, tableIndex))
		{
			//Taking the value pushed by luanext into account
			stackValues++;
			// Remember that Lua arrays are 1-based and not 0-based!
			float tableValue = (float)lua_tonumber(&io_luaState, -1);
			if (!lua_isnil(&io_luaState, -1))
			{
				values[currentValueCount] = tableValue;
				currentValueCount++;
				lua_pop(&io_luaState, 1);
				stackValues--;
			}
			else
			{
				std::cerr << "\tThe value #" << tableValue << " isn't a number!\n";
				lua_pop(&io_luaState, stackValues);
				return false;
			}
		}
	}
	//Popped by luanext
	stackValues--;
	io_vector3.setx(values[0]);
	io_vector3.sety(values[1]);
	io_vector3.setz(values[2]);
	return true;
}

bool LuaHelpers::LoadFloat2(lua_State& io_luaState, int& stackValues, Engine::Vector3& io_vector3)
{
	float values[2];
	int currentValueCount = 0;
	lua_pushnil(&io_luaState);
	stackValues++;
	const int tableIndex = -2;
	{
		while (lua_next(&io_luaState, tableIndex))
		{
			//Taking the value pushed by luanext into account
			stackValues++;
			// Remember that Lua arrays are 1-based and not 0-based!
			float tableValue = (float)lua_tonumber(&io_luaState, -1);
			if (!lua_isnil(&io_luaState, -1))
			{
				values[currentValueCount] = tableValue;
				currentValueCount++;
				lua_pop(&io_luaState, 1);
				stackValues--;
			}
			else
			{
				std::cerr << "\tThe value #" << tableValue << " isn't a number!\n";
				lua_pop(&io_luaState, stackValues);
				return false;
			}
		}
	}
	//Popped by luanext
	stackValues--;
	io_vector3.setx(values[0]);
	io_vector3.sety(values[1]);
	io_vector3.setz(0.0f);
	return true;
}