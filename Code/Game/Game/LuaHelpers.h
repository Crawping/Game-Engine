#include "Vector3.h"
#include "../../External/Lua/Includes.h"
namespace LuaHelpers
{
	bool LoadLuaTable(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaString(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaNumber(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadLuaBoolean(lua_State& io_luaState, const char* i_key, int& stackValues);
	bool LoadFloat3(lua_State& io_luaState, int& stackValues, Engine::Vector3& io_vector3);
	bool LoadFloat2(lua_State& io_luaState, int& stackValues, Engine::Vector3& io_vector3);
}