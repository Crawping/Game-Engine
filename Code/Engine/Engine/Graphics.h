/*
This file contains the function declarations for graphics
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Header Files
//=============

#include "precompiled.h"
#include <d3d9.h>
#include <map>
#include "Actor.h"
#include "Material.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Sprite.h"
#include <string>
#include "LineRenderer.h"
#include "Timer.h"

namespace Engine
{

	class Camera
	{
		Vector3 m_Position;
		Vector3 m_Orientation;
		D3DXMATRIX m_WorldToViewTransform;
	public:
		Vector3& GetPosition(){ return m_Position; }
		Vector3 GetOrientation(){ return m_Orientation; };
		void SetPosition(Vector3 i_Position){ m_Position = i_Position; };
		void SetOrientation(Vector3 i_Orientation){ m_Orientation = i_Orientation; };
		void SetWorldToView()
		{
		
			// D3DX can calculate the inverse of any matrix:
			//D3DXMATRIX transform_worldToView;
			//D3DXMatrixInverse( &transform_worldToView, NULL, &transform_viewToWorld );
			// But it can be calculated more cheaply since we know a camera can only have
			// rotation and translation:

			D3DXMATRIX MatTemp;  // Temp matrix for rotations.
			D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
			D3DXMATRIX MatWorld;

			// Using the left-to-right order of matrix concatenation,
			// apply the translation to the object's world position
			// before applying the rotations.
			D3DXMatrixTranslation(&MatWorld, m_Position.getx(), m_Position.gety(), m_Position.getz());
			D3DXMatrixIdentity(&MatRot);

			D3DXMatrixRotationX(&MatTemp, m_Orientation.getx());         // Pitch
			D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
			D3DXMatrixRotationY(&MatTemp, m_Orientation.gety());           // Yaw
			D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
			D3DXMatrixRotationZ(&MatTemp, m_Orientation.getz());          // Roll
			D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);

			// Apply the rotation matrices to complete the world matrix.
			D3DXMatrixMultiply(&MatWorld, &MatRot, &MatWorld);
			D3DXMatrixInverse(&m_WorldToViewTransform, NULL, &MatWorld);
		}
		D3DXMATRIX* GetWorldToView()
		{
			return &m_WorldToViewTransform;
		}
	};


	class Graphics
	{

		//TODO:REMOVE THIS HACK
		Vector3 m_Velocity;
		HWND s_mainWindow ;
		IDirect3D9* s_direct3dInterface ;
		IDirect3DDevice9* s_direct3dDevice ;

		// Camera
		Camera m_Camera;
		// Debug Font
		ID3DXFont *m_font;
		// View To Screen Matrix
		D3DXMATRIX m_ViewToScreen;

		// The vertex information that is sent to the display adaptor must match what the vertex shader expects
		
		D3DVERTEXELEMENT9 s_vertexElements[5];
		IDirect3DVertexDeclaration9* s_vertexDeclaration;
		IDirect3DVertexBuffer9* s_vertexBuffer[2] ;

		IDirect3DVertexShader9* s_vertexShader ;
		IDirect3DPixelShader9* s_fragmentShader ;

		// The index buffer describes how to make triangles with the vertices
		// (i.e. it defines the vertex connectivity)
		IDirect3DIndexBuffer9* s_indexBuffer[2] ;

		// Private default constructor, copy constructor, and assignment operator
		Graphics();
		Graphics(const Graphics&);
		void operator=(const Graphics&);
		std::map<std::string, Material*> m_MaterialList;
		std::map<std::string, Mesh*> m_MeshList;
		std::vector<LPD3DXMESH*> m_debugMeshList;
		std::vector<Vector3> m_debugMeshPositions;
		std::map<std::string, IDirect3DVertexBuffer9*> m_VertexBufferList;
		std::map<std::string, IDirect3DIndexBuffer9*> m_IndexBufferList;

		LineRenderer debugLines;
		LineRenderer persistentDebugLines;
		// Sprite List
		std::vector<Sprite*> m_SpriteList;
		std::vector<Sprite*> m_DebugSpriteList;

		// Debug Text List
		std::vector<char*> m_debugText;
		std::vector<char*> m_GUIText;
		std::vector<RECT> m_debugTextRects;
		std::vector<RECT> m_GUITextRects;
		
		// Static Collision Mesh
		bool m_enableDebugMenus;
		
		
	public:
		//TODO:REMOVE THIS HACK
		Vector3 GetVelocity(){ return m_Velocity; };

		// Get Single Instance of this class
		static Graphics* GetInstance();
		bool LoadMaterialNamesFile(const char*);
		bool LoadMeshNamesFile(const char*);

		bool InitializeMaterialMap(lua_State& io_luaState);
		bool InitializeMeshMap(lua_State& io_luaState);

		// Assign Meshes and Materials
		bool AssignMaterial(const char* i_matName, SharedPointer<Actor> i_Actor);
		bool AssignMaterial(const char* i_matName, Sprite* i_Sprite);

		bool Engine::Graphics::AssignMesh(const char* i_matName, SharedPointer<Actor> i_Actor);


		// Set Lights
		void SetAmbientLight(Vector3 i_lightColor);
		void SetDiffuseLight(Vector3 i_lightColor);

		// To be executed once at the start of the program
		bool Initialize(const HWND i_mainWindow);
		
		// Setup Camera
		D3DXMATRIX Engine::Graphics::GetWorldToViewTransform();

		// Called every frame to render the game scene
		void Render();

		// Called at the end of the program to exit cleanly
		bool ShutDown();

		//Helper functions
		bool CreateDevice(const HWND i_mainWindow);
		bool CreateInterface(const HWND i_mainWindow);
		bool CreateIndexBuffer(const DWORD i_usage);
		bool CreateVertexAndIndexBuffers();
		bool CreateVertexBuffer(const DWORD i_usage);
		bool LoadFragmentShader();
		bool LoadVertexShader();
		bool LoadMeshData();
		// Sprite List functions
		void addSprite(Sprite* i_Sprite)
		{ 
			i_Sprite->CreateQuadMeshBuffer(s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
			i_Sprite->FillQuadMeshBuffer(s_mainWindow);
			m_SpriteList.push_back(i_Sprite); 
			
		};

		Sprite* getSprite(int i_index)
		{
			return m_SpriteList[i_index];
		}

		// Debug Sprite List functions
		void addDebugSprite(Sprite* i_Sprite)
		{
			i_Sprite->CreateQuadMeshBuffer(s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
			i_Sprite->FillQuadMeshBuffer(s_mainWindow);
			m_DebugSpriteList.push_back(i_Sprite);

		};

		// Add Debug Text to List
		void addDebugText(char* i_newText,RECT i_rect)
		{
			m_debugText.push_back(i_newText);
			m_debugTextRects.push_back(i_rect);
		}

		// Add GUI Text
		void addGUIText(char* i_newText, RECT i_rect)
		{
			m_GUIText.push_back(i_newText);
			m_GUITextRects.push_back(i_rect);
		}

		void clearGUIText()
		{
			m_GUIText.clear();
			m_GUITextRects.clear();
		}
		// Matrix Setters and Getters
		D3DXMATRIX* GetViewToScreenTransform();
		void SetViewToScreen();
		void UpdateLightSource();
		void SetUpProjectile();
		void SetDebugLinesCapacity(int i_cap) 
		{ 
			debugLines.SetLimit(i_cap);
			persistentDebugLines.SetLimit(i_cap);
			// Create Debug Lines Object

			debugLines.SetMaterial(m_MaterialList["data/Material_DebugLines.txt"]);
			persistentDebugLines.SetMaterial(m_MaterialList["data/Material_DebugLines.txt"]);
			debugLines.CreateQuadMeshBuffer(s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
			persistentDebugLines.CreateQuadMeshBuffer(s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);

		};
		Camera* GetCamera(){ return &m_Camera; }
		void SetDebugMenuState(bool i_value){ m_enableDebugMenus = i_value; }
		void AddDebugSphere(float i_radius, Vector3 i_position)
		{
			LPD3DXMESH* sphereMesh = new LPD3DXMESH();
			D3DXCreateSphere(s_direct3dDevice, i_radius, 10, 10, sphereMesh, NULL);
			m_debugMeshList.push_back(sphereMesh);
			m_debugMeshPositions.push_back(i_position);
		}
		struct _VERTEX
		{
			D3DXVECTOR3 pos;     // vertex position
			D3DXCOLOR color;
			D3DXVECTOR3 norm;    // vertex normal
			float tu;            // texture coordinates
			float tv;
		} VERTEX, *LPVERTEX;
		void AddDebugCube(float i_width, float i_height, float i_depth, Vector3 i_position, D3DCOLOR i_color)
		{
			LPD3DXMESH* cubeMesh = new LPD3DXMESH();
			D3DXCreateBox(s_direct3dDevice, i_width, i_height, i_depth,  cubeMesh, NULL);
			
			m_debugMeshPositions.push_back(i_position);

			// create the sphere
			
			// create a copy of the mesh with texture coordinates,
			// since the D3DX function doesn't include them
			LPD3DXMESH* texMesh = new LPD3DXMESH();
			(*cubeMesh)->CloneMeshFVF(D3DXMESH_SYSTEMMEM, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, s_direct3dDevice, texMesh);

			// lock the vertex buffer
			_VERTEX *pVerts;
			(*texMesh)->LockVertexBuffer(0, (LPVOID*)&pVerts);

			// get vertex count
			int numVerts = (*texMesh)->GetNumVertices();

			// loop through the vertices
			for (int i = 0; i<numVerts; i++) {

				// calculate texture coordinates
				pVerts->color = D3DCOLOR_ARGB(255,255,0,0);
				pVerts++;
			}

			// unlock the vertex buffer
			(*texMesh)->UnlockVertexBuffer();
			m_debugMeshList.push_back(texMesh);

		}

		void AddDebugLine(Vector3 i_startPoint, Vector3 i_endPoint, D3DCOLOR i_color =D3DCOLOR_ARGB(255,0,0,0))
		{
			debugLines.AddLine(i_startPoint, i_endPoint, i_color);
			debugLines.FillLineMeshBuffer(s_mainWindow); 
		}

		void AddPersistentDebugLine(Vector3 i_startPoint, Vector3 i_endPoint, D3DCOLOR i_color = D3DCOLOR_ARGB(255, 0, 0, 0))
		{
			persistentDebugLines.AddLine(i_startPoint, i_endPoint, i_color);
			persistentDebugLines.FillLineMeshBuffer(s_mainWindow);
		}

		void ClearPersistentDebugLines()
		{
			persistentDebugLines.ResetBuffer();
		}
	};

}
#endif	// GRAPHICS_H
