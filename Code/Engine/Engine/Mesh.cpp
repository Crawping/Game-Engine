#include "Mesh.h"
#include "UserInput.h"
#include <fstream>
#include <d3d9.h>


bool fileExists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void Engine::Mesh::Update()
{
	if (Engine::UserInput::IsKeyPressed(VK_LEFT))
	{
		SetX(GetX() - 0.1f);
	}
	if (Engine::UserInput::IsKeyPressed(VK_RIGHT))
	{
		SetX(GetX() + 0.1f);
	}
	if (Engine::UserInput::IsKeyPressed(VK_UP))
	{
		SetZ(GetZ() + 0.1f);
	}
	if (Engine::UserInput::IsKeyPressed(VK_DOWN))
	{
		SetZ(GetZ() - 0.1f);
	}
}


bool Engine::Mesh::LoadMeshDataFromFile(const char* i_path, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements)
{
	bool wereThereErrors = false;
	DWORD usage = 0;
	// Our code will only ever write to the buffer
	usage |= D3DUSAGE_WRITEONLY;

	MeshData currentMesh;
	if (!fileExists(i_path))
	{
		MessageBox(i_WindowHandle, "Failed to Load Mesh File", "File not found", MB_OK | MB_ICONERROR);
		return false;
	}
	std::ifstream vertexFile(i_path, std::ifstream::binary);
	vertexFile.seekg(0, vertexFile.end);
	long size = static_cast<long>(vertexFile.tellg());
	vertexFile.seekg(0);

	//Allocate space for file
	char* buffer = new char[size];
	vertexFile.read(buffer, size);
	currentMesh.m_numberOfVertices = reinterpret_cast<uint32_t*>(buffer)[0];
	currentMesh.m_numberOfIndices = reinterpret_cast<uint32_t*>(buffer)[1];
	currentMesh.m_vertexDataStart = reinterpret_cast<size_t*>(buffer + 2 * sizeof(uint32_t));
	currentMesh.m_indexDataStart = reinterpret_cast<size_t*>(buffer + 2 * sizeof(uint32_t)+currentMesh.m_numberOfVertices*sizeof(sVertex));

	//Save vertex and index count
	m_VertexCount = currentMesh.m_numberOfVertices;
	m_IndexCount = currentMesh.m_numberOfIndices;
	// Initialize the vertex format
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

	// Create a vertex buffer for the cube
	{
		const unsigned int bufferSize = currentMesh.m_numberOfVertices * sizeof(sVertex);
		// We will define our own vertex format
		const DWORD useSeparateVertexDeclaration = 0;
		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* const notUsed = NULL;

		result = i_D3DDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
			&m_BufferData.m_VertexBuffer, notUsed);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to create a vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
			return false;
		}
	}
	// Fill the vertex buffer with the rectangle's vertices
	{
		// Before the vertex buffer can be changed it must be "locked"
		sVertex* vertexData;
		{
			const unsigned int lockEntireBuffer = 0;
			const DWORD useDefaultLockingBehavior = 0;
			result = m_BufferData.m_VertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
			if (FAILED(result))
			{
				MessageBox(i_WindowHandle, "DirectX failed to lock the vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
				return false;
			}
		}
		// Fill the buffer
		{

			memcpy(vertexData, currentMesh.m_vertexDataStart, sizeof(sVertex)*currentMesh.m_numberOfVertices);

			// The buffer must be "unlocked" before it can be used
			{
				result = m_BufferData.m_VertexBuffer->Unlock();
				if (FAILED(result))
				{
					MessageBox(i_WindowHandle, "DirectX failed to unlock the vertex buffer", "No Vertex Buffer", MB_OK | MB_ICONERROR);
					return false;
				}
			}
		}
	}
	// Create an index buffer
	{
		// We'll use 32-bit indices in this class to keep things simple
		D3DFORMAT format = D3DFMT_INDEX32;
		unsigned int bufferLength;
		{
			bufferLength = currentMesh.m_numberOfIndices * sizeof(uint32_t);
		}
		D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
		HANDLE* notUsed = NULL;

		HRESULT result = i_D3DDevice->CreateIndexBuffer(bufferLength, usage, format, useDefaultPool,
			&m_BufferData.m_IndexBuffer, notUsed);
		if (FAILED(result))
		{
			MessageBox(i_WindowHandle, "DirectX failed to create an index buffer", "No Index Buffer", MB_OK | MB_ICONERROR);
			return false;
		}
	}
	// Fill the index buffer with the rectangle's triangles' indices
	{
		// Before the index buffer can be changed it must be "locked"
		uint32_t* indices;
		{
			const unsigned int lockEntireBuffer = 0;
			const DWORD useDefaultLockingBehavior = 0;
			const HRESULT result = m_BufferData.m_IndexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
				reinterpret_cast<void**>(&indices), useDefaultLockingBehavior);
			if (FAILED(result))
			{
				MessageBox(i_WindowHandle, "DirectX failed to lock the index buffer", "No Index Buffer", MB_OK | MB_ICONERROR);
				return false;
			}
		}
		// Fill the buffer
		{
			memcpy(indices, currentMesh.m_indexDataStart, sizeof(uint32_t)*currentMesh.m_numberOfIndices);
		}
		// The buffer must be "unlocked" before it can be used
		{
			const HRESULT result = m_BufferData.m_IndexBuffer->Unlock();
			if (FAILED(result))
			{
				MessageBox(i_WindowHandle, "DirectX failed to unlock the index buffer", "No Index Buffer", MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}
	return true;
}

void Engine::Mesh::ClearMesh()
{
	m_BufferData.m_VertexBuffer->Release();
	m_BufferData.m_VertexBuffer = NULL;
	m_BufferData.m_IndexBuffer->Release();
	m_BufferData.m_IndexBuffer = NULL;
}
