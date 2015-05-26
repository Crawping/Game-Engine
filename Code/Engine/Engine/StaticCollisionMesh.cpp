#include "StaticCollisionMesh.h"
#include <fstream>

static bool fileExists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

struct MeshData
{
	uint32_t m_numberOfVertices;
	size_t* m_vertexDataStart;
};


struct sVertex
{
	float x, y, z;
};
bool Engine::StaticCollisionMesh::LoadMeshDataFromFile(const char* i_path, HWND i_WindowHandle)
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
	currentMesh.m_numberOfVertices = m_numberOfVertices= (reinterpret_cast<uint32_t*>(buffer))[0];
	currentMesh.m_vertexDataStart = reinterpret_cast<size_t*>(buffer + sizeof(uint32_t));
	if (m_numberOfVertices == 0)
		m_vertices = nullptr;
	else
		m_vertices = new Vector3[m_numberOfVertices];
	//Save vertex and index count
	m_numberOfVertices = currentMesh.m_numberOfVertices;
	
		
	memcpy(m_vertices, currentMesh.m_vertexDataStart, sizeof(sVertex)*currentMesh.m_numberOfVertices);
		
	

	
	return true;
}