
#include "Material.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include <d3dx9shader.h>


std::string GetLastWindowsError(DWORD* o_optionalErrorCode = NULL);
std::string GetFormattedWindowsError(const DWORD i_errorCode);

// Loads a Table value corresponding to i_key onto the top of the stack
bool LoadLuaTable(lua_State& io_luaState, const char* i_key, int& stackValues)
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
bool LoadLuaString(lua_State& io_luaState, const char* i_key, int& stackValues)
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

// Iterates through the constants in the Float3 table and initializes the corresponding std::Vectors
bool Engine::Material::LoadFloat3Constants(lua_State& io_luaState, int& stackValues, DWORD* i_compiledShader, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{
	m_fragmentShaderConstantTable->SetDefaults(i_D3DDevice);

	lua_pushnil(&io_luaState);	// This tells lua_next() to return the first key
	const int tableIndex = -2;
	{
		while (lua_next(&io_luaState, tableIndex))
		{
			// After lua_next() the key is at -2 and the value is at -1:
			if (lua_type(&io_luaState, -2) != LUA_TSTRING)
			{
				std::cerr << "Key is not a string\n";
				lua_pop(&io_luaState, stackValues);
				return false;
			}
			if (lua_type(&io_luaState, -1) != LUA_TTABLE)
			{
				std::cerr << "Value is not a table ( Float3)\n";
				lua_pop(&io_luaState, stackValues);
				return false;
			}
			{

				const int arrayLength = luaL_len(&io_luaState, -1);
				if (arrayLength > 0)
				{
					std::cout << "Iterating through the ordered values:\n";
					// Remember that Lua arrays are 1-based and not 0-based!
					for (int i = 1; i <= arrayLength; ++i)
					{
						lua_pushinteger(&io_luaState, i);
						lua_gettable(&io_luaState, -2);
						float tableValue = (float)lua_tonumber(&io_luaState, -1);
						if (!lua_isnil(&io_luaState, (int)tableValue))
						{
							m_colorModifier[i - 1] = tableValue;
							lua_pop(&io_luaState, 1);
						}
						else
						{
							std::cerr << "\tThe value #" << i << " isn't a number!\n";
							lua_pop(&io_luaState, stackValues + 1);
							return false;
						}

					}

					std::string constantName = lua_tostring(&io_luaState, -2);
					D3DXHANDLE constantHandle = (m_fragmentShaderConstantTable)->GetConstantByName(NULL, constantName.c_str());
					(m_fragmentShaderConstantTable)->SetFloatArray(i_D3DDevice, constantHandle, m_colorModifier, 3);
					m_fragmentShaderFloat3Constants[constantName] = constantHandle;
				}
				else
				{
					std::cerr << "The asset table doesn't have any ordered values\n";
					return false;
				}
			}

			// To iterate to the next key/value pair, pop the value, but leave the key:
			lua_pop(&io_luaState, 1);
		}
	}
	return true;
}



bool Engine::Material::LoadFragmentShaderConstantVectors(lua_State& io_luaState, DWORD* i_compiledShader, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{
	int stackValueCount = 0;
	//--------------------------------------------
	// LOAD THE CONSTANTS TABLE
	//--------------------------------------------
	{

		if (!LoadLuaTable(io_luaState, "Constants", stackValueCount))
		{
			return false;
		}

	}
	////--------------------------------------------
	// LOAD THE TABLE CONTAINING ALL FLOAT3 CONSTANTS
	//--------------------------------------------
	{

		if (!LoadLuaTable(io_luaState, "Float3", stackValueCount))
		{
			return false;
		}

	}
	//--------------------------------------------
	// LOAD THE FLOAT3 CONSTANTS, SET THEM , AND MAINTAIN A STD::VECTOR WITH HANDLES TO EACH OF THEM
	//--------------------------------------------

	if (!LoadFloat3Constants(io_luaState, stackValueCount, i_compiledShader, i_WindowHandle, i_D3DDevice))
	{
		return false;
	}

	//Pop the ColorModifier Table
	lua_pop(&io_luaState, 1);
	stackValueCount--; 

	// Pop the Float3 Table
	lua_pop(&io_luaState, 1);

	// Make sure stack is returned to its original state
	stackValueCount--;
	assert(stackValueCount == 0);

	return true;
}

bool Engine::Material::LoadShaders(lua_State& io_luaState, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{

	int stackValueCount = 0;
	//--------------------------------------------
	// LOAD THE SHADERS TABLE
	//--------------------------------------------
	{
		if (!LoadLuaTable(io_luaState, "Shaders", stackValueCount))
		{
			return false;
		}

	}
	
	//--------------------------------------------
	// LOAD THE VERTEX SHADER
	//--------------------------------------------
	
	if (!LoadLuaString(io_luaState, "VertexShader", stackValueCount))
	{
		return false;
	}
	{
		const char * vertexShaderPath = const_cast<char*>(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		stackValueCount--;
		std::string errormessage;
		DWORD* compiledShader = NULL;
		void* o_compiledShader = NULL;
		if (!LoadAndAllocateShaderProgram(vertexShaderPath, o_compiledShader, &errormessage))
		{
			std::string errorMessage = std::string("Failed to compile vertex shader ") +
				vertexShaderPath;
			MessageBox(i_WindowHandle, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
			lua_pop(&io_luaState, stackValueCount);
			return false;
		}
		compiledShader = reinterpret_cast<DWORD*>(o_compiledShader);
		HRESULT result = i_D3DDevice->CreateVertexShader((compiledShader),
			&m_vertexShader);
		// (The asset table is now at -1)
		result = D3DXGetShaderConstantTable(compiledShader, &m_vertexShaderConstantTable);
	}
	

	//--------------------------------------------
	// LOAD THE FRAGMENT SHADER
	//--------------------------------------------
	DWORD* compiledShader = NULL;
	if (!LoadLuaString(io_luaState, "FragmentShader", stackValueCount))
	{
		return false;
	}
	{
		const char * fragmentShaderPath = const_cast<char*>(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		stackValueCount--;
		std::string errormessage;
		void* o_compiledShader = NULL;

		if (!LoadAndAllocateShaderProgram(fragmentShaderPath, o_compiledShader, &errormessage))
		{
			std::string errorMessage = std::string("Failed to compile fragment shader ") +
				fragmentShaderPath;
			MessageBox(i_WindowHandle, errorMessage.c_str(), "No Fragment Shader", MB_OK | MB_ICONERROR);
			lua_pop(&io_luaState, stackValueCount);
			lua_pop(&io_luaState, stackValueCount);
			return false;
		}
		compiledShader = reinterpret_cast<DWORD*>(o_compiledShader);
		HRESULT result = i_D3DDevice->CreatePixelShader((compiledShader),
			&m_fragmentShader);
		// (The asset table is now at -1)
		result = D3DXGetShaderConstantTable((compiledShader), &m_fragmentShaderConstantTable);
	}
	

	// Pop the Shaders Table
	lua_pop(&io_luaState, 1);
	stackValueCount--;
	int b = lua_gettop(&io_luaState);
	// Load constant values for fragment shader
	if (!LoadFragmentShaderConstantVectors(io_luaState, compiledShader, i_WindowHandle, i_D3DDevice))
	{
		lua_pop(&io_luaState, stackValueCount);
		return false;
	}
	// Make sure stack is returned to its original state
	int a = lua_gettop(&io_luaState);
	assert(stackValueCount == 0);
	return true;
}

bool Engine::Material::LoadVertexShader(const char* i_VertexShaderPath, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{
	// Load the source code from file and compile it
	ID3DXBuffer* compiledShader;
	{
		const char* sourceCodeFileName = i_VertexShaderPath;
		const D3DXMACRO* noMacros = NULL;
		ID3DXInclude* noIncludes = NULL;
		const char* entryPoint = "main";
		const char* profile = "vs_3_0";
		DWORD flags = 0;
#ifdef EAE6320_GRAPHICS_SHOULDDEBUGSHADERSBEUSED
		flags |=
			// Include debug information in shaders
			D3DXSHADER_DEBUG |
			// Don't do any optimizations to make stepping through the code easier to follow
			D3DXSHADER_SKIPOPTIMIZATION;
#endif
		
		ID3DXBuffer* errorMessages = NULL;
		ID3DXConstantTable** noConstants = NULL;
		HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, noMacros, noIncludes, entryPoint, profile, flags,
			&compiledShader, &errorMessages, noConstants);
		if (SUCCEEDED(result))
		{
			if (errorMessages)
			{
				errorMessages->Release();
			}
		}
		else
		{
			if (errorMessages)
			{
				std::string errorMessage = std::string("DirectX failed to compiled the vertex shader from the file ") +
					sourceCodeFileName + ":\n" +
					reinterpret_cast<char*>(errorMessages->GetBufferPointer());
				MessageBox(i_WindowHandle, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
				errorMessages->Release();
				return false;
			}
			else
			{
				std::string errorMessage = "DirectX failed to compiled the vertex shader from the file ";
				errorMessage += sourceCodeFileName;
				MessageBox(i_WindowHandle, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}
	// Create the vertex shader object
	bool wereThereErrors = false;
	{
		HRESULT result = i_D3DDevice->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
			&m_vertexShader);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to create the vertex shader", "No Vertex Shader", MB_OK | MB_ICONERROR);
			wereThereErrors = true;
		}

		result = D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()), &m_vertexShaderConstantTable);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to get the constants table", "No Vertex Shader Constant Table", MB_OK | MB_ICONERROR);
			wereThereErrors = true;
		}
		compiledShader->Release();
	}
	return !wereThereErrors;
}

bool Engine::Material::LoadFragmentShader(const char* i_FragmentShaderPath, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, ID3DXBuffer** o_compiledShader)
{
	// Load the source code from file and compile it
	ID3DXBuffer* compiledShader;
	{
		const char* sourceCodeFileName = i_FragmentShaderPath;
		const D3DXMACRO* noMacros = NULL;
		ID3DXInclude* noIncludes = NULL;
		const char* entryPoint = "main";
		const char* profile = "ps_3_0";
		DWORD flags = 0;
#ifdef EAE6320_GRAPHICS_SHOULDDEBUGSHADERSBEUSED
		flags |=
			// Include debug information in shaders
			D3DXSHADER_DEBUG |
			// Don't do any optimizations to make stepping through the code easier to follow
			D3DXSHADER_SKIPOPTIMIZATION;
#endif
		ID3DXBuffer* errorMessages = NULL;
		ID3DXConstantTable** noConstants = NULL;
		HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, noMacros, noIncludes, entryPoint, profile, flags,
			&compiledShader, &errorMessages, noConstants);
		if (SUCCEEDED(result))
		{
			if (errorMessages)
			{
				errorMessages->Release();
			}
		}
		else
		{
			if (errorMessages)
			{
				std::string errorMessage = std::string("DirectX failed to compiled the fragment shader from the file ") +
					sourceCodeFileName + ":\n" +
					reinterpret_cast<char*>(errorMessages->GetBufferPointer());
				MessageBox(i_WindowHandle, errorMessage.c_str(), "No Fragment Shader", MB_OK | MB_ICONERROR);
				errorMessages->Release();
				return false;
			}
			else
			{
				std::string errorMessage = "DirectX failed to compiled the fragment shader from the file ";
				errorMessage += sourceCodeFileName;
				MessageBox(i_WindowHandle, errorMessage.c_str(), "No Fragment Shader", MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}
	// Create the fragment shader object
	bool wereThereErrors = false;
	{
		HRESULT result = i_D3DDevice->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
			&m_fragmentShader);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to create the fragment shader", "No Fragment Shader", MB_OK | MB_ICONERROR);
			wereThereErrors = true;
		}
		(*o_compiledShader) = compiledShader;
		return !wereThereErrors;
	}

	
}


HRESULT Engine::Material::Set(IDirect3DDevice9& i_direct3dDevice
#ifdef EAE6320_GRAPHICS_SHOULDALLRETURNVALUESBECHECKED
	, std::string* o_errorMessage = NULL
#endif
	)
{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
	D3DPERF_BeginEvent(0, L"Set Material");
#endif
	HRESULT result = D3D_OK;

	// Set the shaders
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Shaders");
#endif
		result = i_direct3dDevice.SetVertexShader(m_vertexShader);
		assert(SUCCEEDED(result));
#ifdef EAE6320_GRAPHICS_SHOULDALLRETURNVALUESBECHECKED
		if (FAILED(result))
		{
			if (o_errorMessage)
			{
				*o_errorMessage = "Direct3D failed to set the vertex shader";
			}
			return result;
		}
#endif
		result = i_direct3dDevice.SetPixelShader(m_fragmentShader);
		assert(SUCCEEDED(result));

#ifdef EAE6320_GRAPHICS_SHOULDALLRETURNVALUESBECHECKED
		if (FAILED(result))
		{
			if (o_errorMessage)
			{
				*o_errorMessage = "Direct3D failed to set the fragment shader";
			}
			return result;
		}
#endif

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
	}

	// Set the Texture
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
	D3DPERF_BeginEvent(0, L"Set Texture");
#endif
	if (m_hasTexture)
		result = i_direct3dDevice.SetTexture(m_samplerRegister, m_texture);

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
	D3DPERF_EndEvent();
#endif

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
	D3DPERF_EndEvent();
#endif
	return result;
}

// Loads the Materials shaders from the .txt file
bool Engine::Material::LoadMaterialFromFile(const char* i_path, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{
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
		const int luaResult = luaL_dofile(luaState, i_path);
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
			std::string errorMessage = std::string("Failed to load Material File ") + i_path;
			MessageBox(i_WindowHandle, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	if (!LoadShaders(*luaState, i_WindowHandle, i_D3DDevice))
	{
		wereThereErrors = true;
		lua_pop(luaState, 1);
		goto OnExit;
	}

	if (!LoadTexture(*luaState, i_WindowHandle, i_D3DDevice))
	{
		wereThereErrors = true;
		lua_pop(luaState, 1);
		goto OnExit;
	}

	// Pop the table
	lua_pop(luaState, 1);
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

bool Engine::Material::LoadTexture(lua_State& io_luaState, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice)
{
	int stackValues = 0;
	if (!LoadLuaString(io_luaState, "Texture", stackValues))
	{
		return false;
	}

	char * texturePath = const_cast<char*>(lua_tostring(&io_luaState, -1));
	lua_pop(&io_luaState, 1);
	stackValues--;

	HRESULT result = D3DXCreateTextureFromFile(i_D3DDevice, texturePath, &m_texture);
	if (FAILED(result))
	{
		MessageBox(i_WindowHandle, "Failed to load texture", "Texture not found", MB_OK | MB_ICONERROR);
		lua_pop(&io_luaState, stackValues);
		return false;
	}

	D3DXHANDLE samplerHandle = m_fragmentShaderConstantTable->GetConstantByName(NULL,
		"g_color_sampler");
	if (samplerHandle != NULL)
	{
		m_samplerRegister = static_cast<DWORD>(m_fragmentShaderConstantTable->GetSamplerIndex(samplerHandle));
		return true;
	}
	else
	{
		m_hasTexture = false;
		return true;
	}
}

// Releases the shaders and deletes the Material instance
bool Engine::Material::ClearMaterial()
{
	bool wereThereErrors = false;
	if (m_vertexShader)
	{
		if (m_vertexShaderConstantTable)
			m_vertexShaderConstantTable->Release();
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}

	else
	{
		wereThereErrors = true;
	}


	if (m_fragmentShader)
	{
		if (m_fragmentShaderConstantTable)
			m_fragmentShaderConstantTable->Release();
		m_fragmentShader->Release();
		m_fragmentShader = NULL;
	}
	else
	{
		wereThereErrors = true;
	}

	delete this;

	return !wereThereErrors;
}


IDirect3DVertexShader9* Engine::Material::GetVertexShader()
{
	return m_vertexShader;
}
IDirect3DPixelShader9*  Engine::Material::GetFragmentShader()
{
	return m_fragmentShader; 
}

ID3DXConstantTable* Engine::Material::GetVertexShaderConstantTable()
{
	return m_vertexShaderConstantTable;
}

ID3DXConstantTable* Engine::Material::GetFragmentShaderConstantTable()
{
	return m_fragmentShaderConstantTable;
}


//------------------------------
// Load And Compile Shaders
//------------------------------


bool Engine::Material::LoadAndAllocateShaderProgram(const char* i_path, void*& o_compiledShader, std::string* o_errorMessage)
{
	bool wereThereErrors = false;

	// Load the compiled shader from disk
	o_compiledShader = NULL;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	{
		// Open the file
		{
			const DWORD desiredAccess = FILE_GENERIC_READ;
			const DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
			SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
			const DWORD onlySucceedIfFileExists = OPEN_EXISTING;
			const DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
			const HANDLE dontUseTemplateFile = NULL;
			fileHandle = CreateFile(i_path, desiredAccess, otherProgramsCanStillReadTheFile,
				useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile);
			if (fileHandle == INVALID_HANDLE_VALUE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::stringstream errorMessage;
					errorMessage << "Windows failed to open the shader file: " << GetLastWindowsError();
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		// Get the file's size
		size_t fileSize;
		{
			LARGE_INTEGER fileSize_integer;
			if (GetFileSizeEx(fileHandle, &fileSize_integer) != FALSE)
			{
				// This is unsafe if the file's size is bigger than a size_t
				fileSize = static_cast<size_t>(fileSize_integer.QuadPart);
			}
			else
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::stringstream errorMessage;
					errorMessage << "Windows failed to get the size of shader: " << GetLastWindowsError();
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		// Read the file's contents into temporary memory
		o_compiledShader = malloc(fileSize);
		if (o_compiledShader)
		{
			DWORD bytesReadCount;
			OVERLAPPED* readSynchronously = NULL;
			if (ReadFile(fileHandle, o_compiledShader, fileSize,
				&bytesReadCount, readSynchronously) == FALSE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::stringstream errorMessage;
					errorMessage << "Windows failed to read the contents of shader: " << GetLastWindowsError();
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			if (o_errorMessage)
			{
				std::stringstream errorMessage;
				errorMessage << "Failed to allocate " << fileSize << " bytes to read in the shader program " << i_path;
				*o_errorMessage = errorMessage.str();
			}
			goto OnExit;
		}
	}

OnExit:

	if (wereThereErrors && o_compiledShader)
	{
		free(o_compiledShader);
		o_compiledShader = NULL;
	}
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		if (CloseHandle(fileHandle) == FALSE)
		{
			if (!wereThereErrors && o_errorMessage)
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to close the shader file handle: " << GetLastWindowsError();
				*o_errorMessage = errorMessage.str();
			}
			wereThereErrors = true;
		}
		fileHandle = INVALID_HANDLE_VALUE;
	}

	return !wereThereErrors;
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