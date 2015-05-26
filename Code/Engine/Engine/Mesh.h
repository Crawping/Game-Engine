#ifndef MESH_H
#define MESH_H
#include "Vector3.h"
#include <d3d9.h>
#include <stdint.h>
namespace Engine
{
	struct BufferData
	{
		IDirect3DVertexBuffer9* m_VertexBuffer;
		IDirect3DIndexBuffer9* m_IndexBuffer;
	};
	struct MeshData
	{
		uint32_t m_numberOfVertices;
		uint32_t m_numberOfIndices;
		size_t* m_vertexDataStart;
		size_t* m_indexDataStart;
	};

	struct sVertex
	{
		float x, y, z;
		float u, v;
		D3DCOLOR color;
		float nx,ny,nz;
	};
	class Mesh
	{
		unsigned int m_VertexCount;
		unsigned int m_IndexCount;
		Vector3 m_Position;
		BufferData m_BufferData;
	public:
		inline unsigned int GetVertexCount(){ return m_VertexCount; };
		inline unsigned int GetIndexCount(){ return m_IndexCount; };
		inline void SetPosition(const Vector3);
		inline Vector3 GetPosition() const;
		inline void SetX(float i_X);
		inline void SetY(float i_Y);
		inline void SetZ(float i_Z);
		inline float GetX();
		inline float GetY();
		inline float GetZ();
		void Update();
		bool LoadMeshDataFromFile(const char* i_path, HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements);
		BufferData& GetBufferData(){ return m_BufferData; };
		void ClearMesh();
	};
}

#include "Mesh.inl"
#endif //MESH_H