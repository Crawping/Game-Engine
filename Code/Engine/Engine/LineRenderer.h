#ifndef LINERENDERER_H
#define LINERENDERER_H
#include "Vector3.h"
#include <d3d9.h>
#include <vector>
#include "Material.h"
namespace Engine
{
	class LineRenderer
	{
		std::vector<Vector3> m_Lines;
		std::vector<D3DCOLOR> m_Colors;
		IDirect3DVertexBuffer9* m_VertexBuffer;
		Material* m_Material;
		int m_PointCount;
		int m_PointLimit;
		int m_PointsAdded;
		float m_width;
	public:
		LineRenderer() :m_PointCount(0), m_PointsAdded(0){};
		LineRenderer(int i_PointLimit) :m_PointLimit(i_PointLimit), m_PointCount(0), m_PointsAdded(0){};

		void SetMaterial(Material* i_material){ m_Material = i_material; };
		void SetLimit(int i_limit){ m_PointLimit = i_limit; };
		void AddLine(Vector3 i_StartPoint, Vector3 i_EndPoint, D3DCOLOR i_Color=D3DCOLOR_XRGB(255, 255, 255));
		void AddPoint(Vector3 i_Point, D3DCOLOR i_Color = D3DCOLOR_XRGB(255, 255, 255));
		bool CreateQuadMeshBuffer(HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements);
		bool FillQuadMeshBuffer(HWND i_WindowHandle);
		bool FillLineMeshBuffer(HWND i_WindowHandle);
		Material* GetMaterial(){ return m_Material; };
		int GetSize(){ return m_PointCount; };
		void SetWidth(float i_width){ m_width = i_width; }
		IDirect3DVertexBuffer9* GetBufferData(){ return m_VertexBuffer; };
		void ResetBuffer(){ m_PointCount = 0; m_Lines.clear(); m_Colors.clear(); };
	};
}
#endif //LINERENDERER_H