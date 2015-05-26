// Header Files
//=============

#include "MeshBuilder.h"

#include <iostream>
#include <sstream>
#include <assert.h>
#include <iostream>
#include <fstream>
// Interface
//==========

// Build
//------
struct MeshData
{
	uint32_t m_numberOfVertices;
	uint32_t m_numberOfIndices;
	size_t* m_vertexPositions;
	size_t* m_indexPositions;
	size_t* m_colors;
};
bool eae6320::MeshBuilder::Build(const std::vector<const std::string>&)
{
	std::ofstream outfile(m_path_target, std::ofstream::binary);
	std::string MaterialName;
	MeshData cubeData;
	char* dataBuffer;
	unsigned int bufferSize;
	bool wereThereErrors = false;
	lua_State* luaState = NULL;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			std::cerr << "Failed to create a new Lua state\n";
			goto OnExit;
		}
	}

	// Load the asset file into a table at the top of the stack
	{
		const int luaResult = luaL_dofile(luaState, m_path_source);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const int returnedValueCount = lua_gettop(luaState);
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::cerr << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")\n";
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::cerr << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)\n";
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);
			std::string errorMessage = std::string("Failed to load Material File ") + m_path_source;
			//MessageBox(i_WindowHandle, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
		int stackValues = 0;
		//Load VertexNumber onto the stack
		if (!LoadLuaNumber(*luaState, "NumberOfVertices", stackValues))
		{
			goto OnExit;
		}
		//Store it in the struct
		{
			cubeData.m_numberOfVertices = lua_tointeger(luaState, -1);
			lua_pop(luaState, 1);
			stackValues--;
		}

		//Load VertexNumber onto the stack
		if (!LoadLuaNumber(*luaState, "NumberOfIndices", stackValues))
		{
			goto OnExit;
		}
		//Store it in the struct
		{
			cubeData.m_numberOfIndices = lua_tointeger(luaState, -1);
			lua_pop(luaState, 1);
			stackValues--;
		}
		
		//Allocate space for the vertices, indices, and colors
		bufferSize = cubeData.m_numberOfVertices *sizeof(float)* 3 + cubeData.m_numberOfVertices * sizeof(uint32_t)
							+ cubeData.m_numberOfIndices*sizeof(uint32_t)
							+cubeData.m_numberOfVertices*sizeof(float)*2
							+ cubeData.m_numberOfVertices*sizeof(float)*3
							+2 * sizeof(uint32_t);
		unsigned int vertexSize = 9 * sizeof(float);
		dataBuffer = reinterpret_cast<char*> (malloc(bufferSize));

		//Store the number of vertices and number of indices
		reinterpret_cast<uint32_t*>(dataBuffer)[0] = cubeData.m_numberOfVertices;
		reinterpret_cast<uint32_t*>(dataBuffer)[1] = cubeData.m_numberOfIndices;

		//Load Vertices table
		if (!LoadLuaTable(*luaState, "Vertices", stackValues))
		{
			goto OnExit;
		}
		//Load Positions Table
		if (!LoadLuaTable(*luaState, "Positions", stackValues))
		{
			goto OnExit;
		}
		//Read all the Positions
		if (!LoadFloat3WithInterval(*luaState, stackValues, vertexSize, reinterpret_cast<char*>(dataBuffer+2*sizeof(uint32_t))))
		{
			goto OnExit;
		}

		//Pop the Positions table
		lua_pop(luaState, 1);
		stackValues--;
		//Load the Other data table
		if (!LoadLuaTable(*luaState, "OtherData", stackValues))
		{
			goto OnExit;
		}

		//Load the UVs table
		if (!LoadLuaTable(*luaState, "UVs", stackValues))
		{
			goto OnExit;
		}

		//Load the UV data
		if (!LoadFloat2WithInterval(*luaState, stackValues, vertexSize, reinterpret_cast<char*>(dataBuffer + 2 * sizeof(uint32_t)+sizeof(float)* 3 )))
		{
			goto OnExit;
		}

		//Pop the UV table
		lua_pop(luaState, 1);
		stackValues--;

		
		//Load the Colors table
		if (!LoadLuaTable(*luaState, "Colors", stackValues))
		{
			goto OnExit;
		}

		//Load the colors
		if (!LoadColorsWithInterval(*luaState, stackValues, vertexSize, reinterpret_cast<char*>(dataBuffer + 2 * sizeof(uint32_t)+sizeof(float)* 3 + 2* sizeof(float))))
		{
			goto OnExit;
		}

		//Pop the Colors table
		lua_pop(luaState, 1);
		stackValues--;

		//Load the Normals table
		if (!LoadLuaTable(*luaState, "Normals", stackValues))
		{
			goto OnExit;
		}

		//Load the normals
		if (!LoadFloat3WithInterval(*luaState, stackValues, vertexSize, reinterpret_cast<char*>(dataBuffer + 2 * sizeof(uint32_t)+sizeof(float)* 3 + 2 * sizeof(float) + sizeof(float))))
		{
			goto OnExit;
		}

		//Pop the Normals, Otherdata, and Positions tables
		lua_pop(luaState, 3);
		stackValues -= 3;

		//Load the Indices table
		if (!LoadLuaTable(*luaState, "Indices", stackValues))
		{
			goto OnExit;
		}

		//Load the Indices
		if (!LoadInt3WithoutInterval(*luaState, stackValues, reinterpret_cast<char*>(dataBuffer + 2 * sizeof(uint32_t)+cubeData.m_numberOfVertices*sizeof(float)* 3
				+ cubeData.m_numberOfVertices*sizeof(float)* 2 + cubeData.m_numberOfVertices*sizeof(uint32_t)+cubeData.m_numberOfVertices*sizeof(float)*3)))
		{
			goto OnExit;
		}
		
		//Pop the Indices table 
		lua_pop(luaState, 1);
		stackValues--;
	}
	// Pop the table
	lua_pop(luaState, 1);
	outfile.write(reinterpret_cast<char*>(dataBuffer), bufferSize);
OnExit:

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		lua_close(luaState);
		luaState = NULL;
	}

	return !wereThereErrors;


}
