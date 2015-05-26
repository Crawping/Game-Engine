#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>
#include "Quad.h"
#include "Material.h"
namespace Engine
{
	struct SpriteSheetProperties
	{
		int m_widthElements;
		int m_heightElements;
		int m_currentElement;
		SpriteSheetProperties(){};
		SpriteSheetProperties(int i_widthElements, int i_heightElements, int i_currentElement) :m_widthElements(i_widthElements), m_heightElements(i_heightElements), m_currentElement(i_currentElement){};
	
	};

	struct NativeResolution
	{
		int m_width;
		int m_height;
		NativeResolution(){};
		NativeResolution(int i_width, int i_height) :m_width(i_width), m_height(i_height){};
	};
	class Sprite
	{
		Quad m_quad;
		Material* m_material;
		IDirect3DVertexBuffer9* m_VertexBuffer;
		SpriteSheetProperties m_sheetProperties;
		static NativeResolution m_nativeResolution;
		bool m_isRenderable;

	public:
		Sprite() :m_isRenderable(true), m_sheetProperties(SpriteSheetProperties(1, 1, 0)){};
		// Setters
		void SetQuad(const Quad& i_quad){ m_quad = i_quad; }
		void SetMaterial( Material* i_material){ m_material = i_material; }
		bool CreateQuadMeshBuffer(HWND i_WindowHandle, IDirect3DDevice9* i_D3DDevice, IDirect3DVertexDeclaration9* i_VertexDeclaration, D3DVERTEXELEMENT9* i_vertexElements);
		bool FillQuadMeshBuffer(HWND i_WindowHandle);
		inline void SetCurrentSpriteSheetElement(unsigned int i_element);
		inline void SetSpriteSheetProperties(SpriteSheetProperties i_spriteSheetProperties);
		inline static void SetNativeResolution(NativeResolution i_nativeResolution);
		inline void SetRenderable(bool i_value){ m_isRenderable = i_value; }
		inline static void AdjustDimensionsForResolution(Quad* i_quad);

		// Getters
		inline static NativeResolution GetNativeResolution();
		inline bool GetRenderable(){ return m_isRenderable; }
		inline SpriteSheetProperties GetSpriteSheetProperties();
		Material* GetMaterial(){ return m_material; };
		Quad* GetQuad(){ return &m_quad; };
		IDirect3DVertexBuffer9* GetBufferData(){ return m_VertexBuffer; };
	};
}
#include "Sprite.inl"
#endif //SPRITE_H