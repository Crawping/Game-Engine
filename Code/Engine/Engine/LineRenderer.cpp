#include "LineRenderer.h"

struct sVertex
{
	float x, y, z;
	float u, v;
	D3DCOLOR color;
	float nx, ny, nz;
};

bool Engine::LineRenderer::CreateQuadMeshBuffer(HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements)
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
		const unsigned int bufferSize = m_PointLimit * sizeof(sVertex);
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


bool Engine::LineRenderer::FillLineMeshBuffer(HWND i_WindowHandle)
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

			for (int i = m_PointCount; i < m_PointCount + m_PointsAdded; i++)
			{

				vertexData[i].x = m_Lines[i].getx();
				vertexData[i].y = m_Lines[i].gety();
				vertexData[i].z = m_Lines[i].getz();

				vertexData[i].color = m_Colors[i];

			}

			m_PointCount += m_PointsAdded;
			m_PointsAdded = 0;
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


bool Engine::LineRenderer::FillQuadMeshBuffer(HWND i_WindowHandle)
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
			int currentTriangle = m_PointCount*6;
			for (int i = m_PointCount; i < m_PointCount + m_PointsAdded-1; i+=2)
			{
				
				vertexData[currentTriangle].x = m_Lines[i].getx() - m_width;
				vertexData[currentTriangle].y = m_Lines[i].gety();
				vertexData[currentTriangle].z = m_Lines[i].getz();
				vertexData[currentTriangle].color = m_Colors[i];
				vertexData[currentTriangle].u = 0.0f;
				vertexData[currentTriangle].v = 0.0f;
				currentTriangle++;

				vertexData[currentTriangle].x = m_Lines[i + 1].getx() + m_width;
				vertexData[currentTriangle].y = m_Lines[i + 1].gety();
				vertexData[currentTriangle].z = m_Lines[i + 1].getz();
				vertexData[currentTriangle].color = m_Colors[i+1];
				vertexData[currentTriangle].u = 1.0f;
				vertexData[currentTriangle].v = 1.0f;
				currentTriangle++;

				vertexData[currentTriangle].x = m_Lines[i].getx() + m_width;
				vertexData[currentTriangle].y = m_Lines[i].gety();
				vertexData[currentTriangle].z = m_Lines[i].getz();
				vertexData[currentTriangle].color = m_Colors[i];
				vertexData[currentTriangle].u = 1.0f;
				vertexData[currentTriangle].v = 0.0f;
				currentTriangle++;

				vertexData[currentTriangle].x = m_Lines[i].getx() - m_width;
				vertexData[currentTriangle].y = m_Lines[i].gety();
				vertexData[currentTriangle].z = m_Lines[i].getz();
				vertexData[currentTriangle].color = m_Colors[i];
				vertexData[currentTriangle].u = 0.0f;
				vertexData[currentTriangle].v = 0.0f;
				currentTriangle++;

				vertexData[currentTriangle].x = m_Lines[i + 1].getx() - m_width;
				vertexData[currentTriangle].y = m_Lines[i + 1].gety();
				vertexData[currentTriangle].z = m_Lines[i + 1].getz();
				vertexData[currentTriangle].color = m_Colors[i + 1];
				vertexData[currentTriangle].u = 0.0f;
				vertexData[currentTriangle].v = 1.0f;
				currentTriangle++;

				vertexData[currentTriangle].x = m_Lines[i + 1].getx() + m_width;
				vertexData[currentTriangle].y = m_Lines[i + 1].gety();
				vertexData[currentTriangle].z = m_Lines[i + 1].getz();
				vertexData[currentTriangle].color = m_Colors[i + 1];
				vertexData[currentTriangle].u = 1.0f;
				vertexData[currentTriangle].v = 1.0f;
				currentTriangle++;
			}

			m_PointCount += m_PointsAdded;
			m_PointsAdded = 0;
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

void Engine::LineRenderer::AddLine(Vector3 i_StartPoint, Vector3 i_EndPoint, D3DCOLOR i_Color)
{
	m_Lines.push_back(i_StartPoint);
	m_Lines.push_back(i_EndPoint);
	m_Colors.push_back(i_Color);
	m_Colors.push_back(i_Color);
	m_PointsAdded+=2;
}

void Engine::LineRenderer::AddPoint(Vector3 i_Point, D3DCOLOR i_Color)
{
	m_Lines.push_back(i_Point);
	m_Colors.push_back(i_Color);
	m_PointsAdded++;
}
