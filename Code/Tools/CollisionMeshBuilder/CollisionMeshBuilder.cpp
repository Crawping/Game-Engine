// Header Files
//=============

#include "CollisionMeshBuilder.h"

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
	size_t* m_vertexPositions;
};
bool eae6320::CollisionMeshBuilder::Build(const std::vector<const std::string>&)
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


		//Allocate space for the vertices, indices, and colors
		bufferSize = cubeData.m_numberOfVertices *sizeof(float)* 3 
			+ sizeof(uint32_t);
		unsigned int vertexSize = 3 * sizeof(float);
		dataBuffer = reinterpret_cast<char*> (malloc(bufferSize));

		//Store the number of vertices and number of indices
		reinterpret_cast<uint32_t*>(dataBuffer)[0] = cubeData.m_numberOfVertices;

		//Load Vertices table
		if (!LoadLuaTable(*luaState, "Vertices", stackValues))
		{
			goto OnExit;
		}
		//Read all the Vertices
		if (!LoadFloat3WithInterval(*luaState, stackValues, 3*sizeof(float), reinterpret_cast<char*>(dataBuffer +  sizeof(uint32_t))))
		{
			goto OnExit;
		}

		//Pop the Vertices table
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
