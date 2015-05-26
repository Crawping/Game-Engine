#include "Sprite.h"
#include "UserSettings.h"

Engine::NativeResolution Engine::Sprite::m_nativeResolution;

struct sVertex
{
	float x, y, z;
	float u, v;
	D3DCOLOR color;
	float nx, ny, nz;
};

bool Engine::Sprite::CreateQuadMeshBuffer(HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements)
{

	

	DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
	HRESULT result = i_D3DDevice->CreateVertexDeclaration(i_vertexElements, &i_VertexDeclaration);
	if (SUCCEEDED(result))
	{
		result = i_D3DDevice->SetVertexDeclaration(i_VertexDeclaration);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to set the vertex declaration", "No Vertex Declaration", MB_OK | MB_ICONERROR);
			return false;
		}
	}
	else
	{
		MessageBox(i_WindowHandle, "DirectX failed to create a Direct3D9 vertex declaration", "No Vertex Declaration", MB_OK | MB_ICONERROR);
		return false;
	}

	// Create a vertex buffer for the quad
	{
		const unsigned int bufferSize = 4* sizeof(sVertex);
		// We will define our own vertex format
		const DWORD useSeparateVertexDeclaration = 0;
		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* const notUsed = NULL;

		result = i_D3DDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
			&m_VertexBuffer, notUsed);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to create a vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
			return false;
		}
	}
	return true;
	
}



bool Engine::Sprite::FillQuadMeshBuffer(HWND i_WindowHandle)
{
	HRESULT result;
	// Fill the vertex buffer with the quads's vertices
	{
		// Before the vertex buffer can be changed it must be "locked"
		sVertex* vertexData;
		{
			const unsigned int lockEntireBuffer = 0;
			const DWORD useDefaultLockingBehavior = 0;
			result = m_VertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
			if (FAILED(result))
			{
				MessageBox(i_WindowHandle, "DirectX failed to lock the vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
				return false;
			}
		}
		// Fill the buffer
		{

			vertexData[0].x = m_quad.getTopLeftX() ;
			vertexData[0].y = m_quad.getTopLeftY();
			vertexData[0].z = 0.0f;
			vertexData[0].u = 0.0f + (float)m_sheetProperties.m_currentElement / (m_sheetProperties.m_widthElements);
			vertexData[0].v = 0.0f + (float)m_sheetProperties.m_currentElement / (m_sheetProperties.m_heightElements);

			vertexData[1].x = m_quad.getTopLeftX() + m_quad.getWidth();
			vertexData[1].y = m_quad.getTopLeftY();
			vertexData[1].z = 0.0f;
			vertexData[1].u = (1.0f + m_sheetProperties.m_currentElement) / (m_sheetProperties.m_widthElements);
			vertexData[1].v = 0.0f + (float)m_sheetProperties.m_currentElement / (m_sheetProperties.m_heightElements);

			vertexData[2].x = m_quad.getTopLeftX();
			vertexData[2].y = m_quad.getTopLeftY() - m_quad.getHeight();
			vertexData[2].z = 0.0f;
			vertexData[2].u = 0.0f + (float)m_sheetProperties.m_currentElement / (m_sheetProperties.m_widthElements);
			vertexData[2].v = (1.0f + m_sheetProperties.m_currentElement)/ (m_sheetProperties.m_heightElements);

			vertexData[3].x = m_quad.getTopLeftX() + m_quad.getWidth();
			vertexData[3].y = m_quad.getTopLeftY() - m_quad.getHeight();
			vertexData[3].z = 0.0f;
			vertexData[3].u = (1.0f + m_sheetProperties.m_currentElement) / (m_sheetProperties.m_widthElements);
			vertexData[3].v = (1.0f + m_sheetProperties.m_currentElement) / (m_sheetProperties.m_heightElements);

			// The buffer must be "unlocked" before it can be used
			{
				result = m_VertexBuffer->Unlock();
				if (FAILED(result))
				{
					MessageBox(i_WindowHandle, "DirectX failed to unlock the vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
					return false;
				}
			}
		}
	}
	return true;
}

