#ifndef MATERIAL_H
#define MATERIAL_H

#include "precompiled.h"
#include <d3d9.h>
#include <d3dx9shader.h>
#include <string>
#include <stdint.h>
#include "Vector3.h"
#include "../../External/Lua/Includes.h"
#include <vector>
#include <map>
struct ID3DXConstantTable;
namespace Engine
{

	class Material
	{

		// Each Material will have a vertex and a fragment shader
		IDirect3DVertexShader9* m_vertexShader;
		ID3DXConstantTable * m_vertexShaderConstantTable;

		std::map<std::string, D3DXHANDLE> m_vertexShaderFloat3Constants;
		IDirect3DPixelShader9* m_fragmentShader;
		ID3DXConstantTable * m_fragmentShaderConstantTable;
		std::map<std::string, D3DXHANDLE> m_fragmentShaderFloat3Constants;

		// Each Material will have a corresponding texture and an associated sampler register
		IDirect3DTexture9* m_texture;;
		DWORD m_samplerRegister;
		bool m_hasTexture;
		float m_colorModifier[3];

	public:

		Material(){ m_vertexShader = NULL; m_fragmentShader = NULL; m_vertexShaderConstantTable = NULL; m_fragmentShaderConstantTable = NULL; m_hasTexture = true; }

		// Set the vertex and fragment shaders to the Material's respective shaders
		HRESULT Set(IDirect3DDevice9& i_direct3dDevice
#ifdef EAE6320_GRAPHICS_SHOULDALLRETURNVALUESBECHECKED
			, std::string* o_errorMessage = NULL
#endif
			);


		HRESULT Load(const char* i_path, IDirect3DDevice9& i_direct3dDevice,
			std::string* o_errorMessage );
		bool LoadMaterialFromFile(const char* , HWND , IDirect3DDevice9* );
		bool LoadShaders(lua_State& , HWND , IDirect3DDevice9* );
		bool LoadVertexShader(const char*, HWND, IDirect3DDevice9*);
		bool LoadFragmentShader( const char*, HWND, IDirect3DDevice9*, ID3DXBuffer** o_compiledShader);
		IDirect3DVertexShader9* GetVertexShader();
		IDirect3DPixelShader9* GetFragmentShader();
		ID3DXConstantTable* GetVertexShaderConstantTable();
		ID3DXConstantTable* GetFragmentShaderConstantTable();
		bool LoadFloat3Constants(lua_State& io_luaState, int& stackValues, DWORD* i_compiledShader, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice);
		bool LoadFragmentShaderConstantVectors(lua_State& io_luaState, DWORD* i_compiledShader, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice);
		bool LoadAndAllocateShaderProgram(const char* i_path, void*& o_compiledShader, std::string* o_errorMessage);
		bool LoadTexture(lua_State& io_luaState, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice);
		bool ClearMaterial();
		float* GetColorModifier(){ return m_colorModifier; };
		void SetColorModifier(Vector3 i_color)
		{
			m_colorModifier[0] = i_color.getx();
			m_colorModifier[1] = i_color.gety();
			m_colorModifier[2] = i_color.getz();
		};

	};

}
#endif //MATERIAL_H