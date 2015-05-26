// Header Files
//=============

#include "precompiled.h"
#include "Graphics.h"
#include <fstream> 
#ifdef _DEBUG
#define EAE6320_GRAPHICS_SHOULDD3DDEBUGINFOBEENABLED
#endif
#ifdef EAE6320_GRAPHICS_SHOULDD3DDEBUGINFOBEENABLED
#define D3D_DEBUG_INFO
#endif

#include <cstdint>
#include <cassert>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <string>
#include <iostream>
#include "Material.h"
#include "UserInput.h"
#include "Time.h"
#include "Mesh.h"
#include "Actor.h"
#include "World.h"
#include "UserSettings.h"
#include "LightSource.h"
#include "Utils.h"
#include "Sprite.h"
#include "LineRenderer.h"

// Defining Keycodes
#define VK_W         0x77
#define VK_S		 0x73
#define VK_A         0x61
#define VK_D         0x64


Engine::LineRenderer projectilePrediction(1000);
Engine::Vector3 distance;
Engine::Vector3 acceleration(0.0f, -9.8f, 0.0f);
float shotPower = 20.0f;

Engine::Vector3 Velocity = Engine::Vector3(1.0f, 3.0f, 1.0f).normalized()*shotPower;
float time = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;
bool enableDebugLines = false;
// Global light source
Engine::LightSource g_lightSource(Engine::Vector3(0.0f, -1.0f, 0.0f), Engine::Vector3(1.0f, 1.0f, 1.0f), Engine::Vector3(0.0f, 10.0f, 0.0f));

// Ambient light 
Engine::LightSource g_ambientLight(Engine::Vector3(0.0f, 0.0f, 0.0f), Engine::Vector3(0.2f, 0.2f, 0.2f), Engine::Vector3(0.0f, 0.0f, 0.0f));

// Default constructor initialized the vertex array
Engine::Graphics::Graphics()
{
	s_vertexElements[0] = { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
	s_vertexElements[1] = { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
	s_vertexElements[2] = { 0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 };
	s_vertexElements[3] = { 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 };
	s_vertexElements[4] = D3DDECL_END();
}

// Returns single instance
Engine::Graphics* Engine::Graphics::GetInstance()
{
	static Graphics SingleInstance;
	return &SingleInstance;
}


bool Engine::Graphics::Initialize(const HWND i_mainWindow)
{
	m_Camera.SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	m_Camera.SetOrientation(Vector3(0.0f, 0.0f, 0.0f));
	// Initialize the interface to the Direct3D9 library
	if (!CreateInterface(i_mainWindow))
	{
		return false;
	}

	// Create an interface to a Direct3D device
	if (CreateDevice(i_mainWindow))
	{
		s_mainWindow = i_mainWindow;
	}
	else
	{
		goto OnError;
	}

#ifdef DEBUG_MENU_ENABLED
	HRESULT result = D3DXCreateFont(s_direct3dDevice,     //D3D Device

		22,               //Font height

		0,                //Font width

		FW_NORMAL,        //Font Weight

		1,                //MipLevels

		false,            //Italic

		DEFAULT_CHARSET,  //CharSet

		OUT_DEFAULT_PRECIS, //OutputPrecision

		ANTIALIASED_QUALITY, //Quality

		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily

		"Arial",          //pFacename,

		&m_font);         //ppFont
	if (!SUCCEEDED(result))
		return false;
#endif

	// Initialize Material List
	if (!LoadMaterialNamesFile("data/materialList.txt"))
	{
		goto OnError;
	}

	// Initialize the Mesh List
	if (!LoadMeshNamesFile("data/meshList.txt"))
	{
		goto OnError;
	}



	// Enable linear filtering for textures

	for (DWORD i = 0; i < 8; ++i)
	{
		s_direct3dDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		s_direct3dDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		s_direct3dDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}


	// Enable Alpha Blending
	s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	s_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	s_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// Set up projectile
	projectilePrediction.CreateQuadMeshBuffer(s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
	SetUpProjectile();
	// Create light source

	m_Camera.SetWorldToView();
	SetViewToScreen();
	return true;


OnError:

	return false;
}

void Engine::Graphics::SetUpProjectile()
{
	projectilePrediction.ResetBuffer();
	float timeSpace = 8.0f;
	int samples = 100;

	float timeInterval = (float)timeSpace / (float)samples;

	// Caculate the arc, find first tile collision, and draw reticle on it
	for (int i = 0; i < samples; i++)
	{
		distance = (Velocity*(i*timeInterval)) + (acceleration*((i*timeInterval)*(i*timeInterval) / 2.0f));
		projectilePrediction.AddPoint(distance);
	}
	projectilePrediction.SetWidth(0.1f);
	projectilePrediction.SetMaterial(m_MaterialList["data/Material_LineRenderer.txt"]);
	projectilePrediction.FillQuadMeshBuffer(s_mainWindow);

}
void Engine::Graphics::Render()
{

	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		const D3DRECT* subRectanglesToClear = NULL;
		const DWORD subRectangleCount = 0;
		const DWORD clearColorAndDepth = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor;
		const D3DCOLOR colorToClear = D3DCOLOR_XRGB(0, 0, 0);
		const float depthToClear = 1.0f;
		// Black is usually used:
		clearColor = D3DCOLOR_XRGB(0, 0, 0);
		const float noZBuffer = 0.0f;
		const DWORD noStencilBuffer = 0;
		HRESULT result = s_direct3dDevice->Clear(subRectangleCount, subRectanglesToClear,
			clearColorAndDepth, colorToClear, depthToClear, noStencilBuffer);
		assert(SUCCEEDED(result));
	}

	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	HRESULT result = s_direct3dDevice->BeginScene();
	assert(SUCCEEDED(result));
	{

#pragma region DrawingMeshes
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Drawing Meshes");
#endif
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Disabling Aplha Blending");
#endif

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		// Enable depth writing and depth comparision

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Enabling Depth Writing and Comparision");
#endif
		s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);



#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		// Loop through all actors in the world and render them
		{

			for (unsigned int i = 0; i < World::getRenderableList().size(); i++)
			{
				SharedPointer<Actor> currentActor = World::getRenderableList()[i];

				if (!currentActor->GetIsRenderable())
					continue;

				// Set the shaders based on the Material of the object being rendered
				{
					currentActor->GetMaterial()->Set(*s_direct3dDevice);
				}

				// Set Constants

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Set Constants And Streams");
#endif

				ID3DXConstantTable* vertexShaderConstTable = currentActor->GetMaterial()->GetVertexShaderConstantTable();
				D3DXHANDLE constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_worldToView");
				//float pos[2] = { TestMesh.GetX(), TestMesh.GetY() };
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, m_Camera.GetWorldToView());
				constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_viewToScreen");
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, GetViewToScreenTransform());
				constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_modelToWorld");
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, &currentActor->GetTransform());


				ID3DXConstantTable* fragmentShaderConstTable = currentActor->GetMaterial()->GetFragmentShaderConstantTable();
				constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_light_ambient");
				float ambientLight[3] = { g_ambientLight.getColor().getx(), g_ambientLight.getColor().gety(), g_ambientLight.getColor().getz() };
				(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, ambientLight, 3);

				constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_light_direction");
				float lightDirection[3] = { g_lightSource.getDirection().getx(), g_lightSource.getDirection().gety(), g_lightSource.getDirection().getz() };
				(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, lightDirection, 3);

				constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_light_color");
				float lightColor[3] = { g_lightSource.getColor().getx(), g_lightSource.getColor().gety(), g_lightSource.getColor().getz() };
				(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, lightColor, 3);

				constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_colorModifier");
				(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, currentActor->GetMaterial()->GetColorModifier(), 3);

				float timeCounter = time / 20;
				constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_time_count");
				(fragmentShaderConstTable)->SetFloat(s_direct3dDevice, constantHandle, timeCounter);


				// Bind a specific vertex buffer to the device as a data source
				{
					// There can be multiple streams of data feeding the display adaptor at the same time
					const unsigned int streamIndex = 0;
					// It's possible to start streaming data in the middle of a vertex buffer
					const unsigned int bufferOffset = 0;
					// The "stride" defines how large a single vertex is in the stream of data
					const unsigned int bufferStride = sizeof(sVertex);
					result = s_direct3dDevice->SetStreamSource(streamIndex, currentActor->GetMesh()->GetBufferData().m_VertexBuffer, bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}
				// Set the indices to use
				{
					HRESULT result = s_direct3dDevice->SetIndices(currentActor->GetMesh()->GetBufferData().m_IndexBuffer);
					assert(SUCCEEDED(result));
				}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
				// Render objects from the current streams
				{
					// We are using triangles as the "primitive" type,
					// and we have defined the vertex buffer as a triangle list
					// (meaning that every triangle is defined by three vertices)
					const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
					// It's possible to start rendering primitives in the middle of the stream
					const unsigned int indexOfFirstVertexToRender = 0;
					const unsigned int indexOfFirstIndexToUse = 0;
					// EAE6320_TODO: How many vertices are in a rectangle,
					// and how many "primitives" (triangles) is it made up of?
					const unsigned int vertexCountToRender = currentActor->GetMesh()->GetVertexCount();
					const unsigned int primitiveCountToRender = currentActor->GetMesh()->GetIndexCount() / 3;
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_BeginEvent(0, L"Mesh DrawCall");
#endif
					HRESULT result = s_direct3dDevice->DrawIndexedPrimitive(primitiveType, indexOfFirstVertexToRender, indexOfFirstVertexToRender,
						vertexCountToRender, indexOfFirstIndexToUse, primitiveCountToRender);
					assert(SUCCEEDED(result));

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_EndEvent();
#endif
				}
			}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif
#pragma endregion DrawingMeshes
			//Enable Wireframe for debug meshes
#ifdef DEBUG_MENU_ENABLED
			s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

#pragma region DrawingDebugMeshes
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Drawing Debug Meshes");
#endif

			// Loop through debug mesh list and draw them all


			for (unsigned int i = 0; i < m_debugMeshList.size(); i++)
			{
				Material* debugMaterial = m_MaterialList["data/cubeMaterial.txt"];
				m_MaterialList["data/cubeMaterial.txt"]->Set(*s_direct3dDevice);

				D3DXMATRIX transform;
				{
					D3DXMatrixTransformation(&transform, NULL, NULL, NULL, NULL,
						NULL, &m_debugMeshPositions[i].getD3DVector3());
				}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Set Constants And Streams");
#endif
				ID3DXConstantTable* vertexShaderConstTable = debugMaterial->GetVertexShaderConstantTable();
				D3DXHANDLE constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_worldToView");
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, m_Camera.GetWorldToView());
				constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_viewToScreen");
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, GetViewToScreenTransform());
				constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_modelToWorld");
				(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, &transform);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Debug Mesh DrawCall");
#endif
				(*m_debugMeshList[i])->DrawSubset(0);

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
			}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif
#pragma endregion DrawingDebugMeshes
#endif
			//Disable Wireframe to render sprites and debug lines 
			s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			s_direct3dDevice->CreateVertexDeclaration(s_vertexElements, &s_vertexDeclaration);
			s_direct3dDevice->SetVertexDeclaration(s_vertexDeclaration);

#pragma region DrawingGUI
			
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Drawing GUI");
#endif


			// Set the Material
			for (unsigned int i = 0; i < m_GUIText.size(); i++)
			{

				const char *text = m_GUIText[i];
				RECT textRect = m_GUITextRects[i];

				//A pre-formatted string showing the current frames per second
#ifdef DEBUG_MENU_ENABLED
				int font_height = m_font->DrawText(NULL,        //pSprite

					text,  //pString

					-1,          //Count

					&textRect,  //pRect

					DT_LEFT | DT_NOCLIP,//Format,

					0xFFFFFFFF); //Color
#endif
			}

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
#pragma endregion DrawingGUI

#ifdef DEBUG_MENU_ENABLED
#pragma region DrawingDebugMenu
			if (m_enableDebugMenus)
			{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Drawing Debug Menus");
#endif


				// Set the Material
				for (unsigned int i = 0; i < m_debugText.size(); i++)
				{

					const char *text = m_debugText[i];
					RECT textRect = m_debugTextRects[i];

					//A pre-formatted string showing the current frames per second
#ifdef DEBUG_MENU_ENABLED
					int font_height = m_font->DrawText(NULL,        //pSprite

						text,  //pString

						-1,          //Count

						&textRect,  //pRect

						DT_LEFT | DT_NOCLIP,//Format,

						0xFFFFFFFF); //Color
#endif
				}
				m_Velocity = Velocity;

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
			}
#pragma endregion DrawingDebugMenu
#pragma region DrawingDebugLines
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Drawing Debug Lines");
#endif
			{
				// Set the Material
				debugLines.GetMaterial()->Set(*s_direct3dDevice);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Setting constants and streams");
#endif
				// Set the constants
				{

					ID3DXConstantTable* vertexShaderConstTable = debugLines.GetMaterial()->GetVertexShaderConstantTable();
					D3DXHANDLE constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_worldToView");
					(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, m_Camera.GetWorldToView());

					constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_viewToScreen");
					(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, GetViewToScreenTransform());

					float timeCounter = time / 60;
					ID3DXConstantTable* fragmentShaderConstTable = debugLines.GetMaterial()->GetFragmentShaderConstantTable();
					constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_time_count");
					(fragmentShaderConstTable)->SetFloat(s_direct3dDevice, constantHandle, timeCounter);
				}



				// Bind a specific vertex buffer to the device as a data source
				{
					// There can be multiple streams of data feeding the display adaptor at the same time
					const unsigned int streamIndex = 0;
					// It's possible to start streaming data in the middle of a vertex buffer
					const unsigned int bufferOffset = 0;
					// The "stride" defines how large a single vertex is in the stream of data
					const unsigned int bufferStride = sizeof(sVertex);
					result = s_direct3dDevice->SetStreamSource(streamIndex, debugLines.GetBufferData(), bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
				const D3DPRIMITIVETYPE lineList = D3DPT_LINELIST;
				const unsigned int indexOfFirstVertexToRender = 0;
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Line Renderer Draw Call");
#endif
				result = s_direct3dDevice->DrawPrimitive(lineList, indexOfFirstVertexToRender, debugLines.GetSize() - 1);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif

			}



#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif
#pragma endregion DrawingDebugLines
#pragma region DrawingPersistentDebugLines
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Drawing Persistent Debug Lines");
#endif
			{
				// Set the Material
				persistentDebugLines.GetMaterial()->Set(*s_direct3dDevice);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Setting constants and streams");
#endif
				// Set the constants
				{

					ID3DXConstantTable* vertexShaderConstTable = persistentDebugLines.GetMaterial()->GetVertexShaderConstantTable();
					D3DXHANDLE constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_worldToView");
					(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, m_Camera.GetWorldToView());

					constantHandle = vertexShaderConstTable->GetConstantByName(NULL, "g_transform_viewToScreen");
					(vertexShaderConstTable)->SetMatrix(s_direct3dDevice, constantHandle, GetViewToScreenTransform());

					float timeCounter = time / 60;
					ID3DXConstantTable* fragmentShaderConstTable = persistentDebugLines.GetMaterial()->GetFragmentShaderConstantTable();
					constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_time_count");
					(fragmentShaderConstTable)->SetFloat(s_direct3dDevice, constantHandle, timeCounter);
				}



				// Bind a specific vertex buffer to the device as a data source
				{
					// There can be multiple streams of data feeding the display adaptor at the same time
					const unsigned int streamIndex = 0;
					// It's possible to start streaming data in the middle of a vertex buffer
					const unsigned int bufferOffset = 0;
					// The "stride" defines how large a single vertex is in the stream of data
					const unsigned int bufferStride = sizeof(sVertex);
					result = s_direct3dDevice->SetStreamSource(streamIndex, persistentDebugLines.GetBufferData(), bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
				const D3DPRIMITIVETYPE lineList = D3DPT_LINELIST;
				const unsigned int indexOfFirstVertexToRender = 0;
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Line Renderer Draw Call");
#endif
				result = s_direct3dDevice->DrawPrimitive(lineList, indexOfFirstVertexToRender, persistentDebugLines.GetSize() - 1);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif

			}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif
#pragma endregion DrawingPersistentDebugLines
#endif
#pragma region DrawingSprites
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Drawing Sprites");
#endif
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Enabling Aplha Blending");
#endif

			// Enable Alpha Blending
			s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			s_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			s_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_BeginEvent(0, L"Turning Off depth comparision");
#endif

			// Turn off depth comparision and depth writing

			s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif

			// Loop through the Sprite List and render all sprites
			for (unsigned int i = 0; i < m_SpriteList.size(); i++)
			{
				Sprite* currentSprite = m_SpriteList[i];
				if (!currentSprite->GetRenderable()) continue;
				// Set the shaders based on the Material of the object being rendered
				{
					currentSprite->GetMaterial()->Set(*s_direct3dDevice);
				}


#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Setting Streams");
#endif

				ID3DXConstantTable* fragmentShaderConstTable = currentSprite->GetMaterial()->GetFragmentShaderConstantTable();
				D3DXHANDLE constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_colorModifier");
				(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, currentSprite->GetMaterial()->GetColorModifier(), 3);
				// Bind a specific vertex buffer to the device as a data source
				{
					// There can be multiple streams of data feeding the display adaptor at the same time
					const unsigned int streamIndex = 0;
					// It's possible to start streaming data in the middle of a vertex buffer
					const unsigned int bufferOffset = 0;
					// The "stride" defines how large a single vertex is in the stream of data
					const unsigned int bufferStride = sizeof(sVertex);
					result = s_direct3dDevice->SetStreamSource(streamIndex, currentSprite->GetBufferData(), bufferOffset, bufferStride);
					assert(SUCCEEDED(result));

				}

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_EndEvent();
#endif
				// Render objects from the current streams
				{

					const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLESTRIP;
					const unsigned int indexOfFirstVertexToRender = 0;
					const unsigned int primitiveCountToRender = 2;
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_BeginEvent(0, L"Sprite DrawCall");
#endif
					HRESULT result = s_direct3dDevice->DrawPrimitive(primitiveType, indexOfFirstVertexToRender, primitiveCountToRender);
					assert(SUCCEEDED(result));

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_EndEvent();
#endif
				}
			}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();
#endif
#pragma endregion DrawingSprites
#ifdef DEBUG_MENU_ENABLED
#pragma region DrawingDebugSprites
			if (m_enableDebugMenus)
			{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
				D3DPERF_BeginEvent(0, L"Drawing Debug Sprites");
#endif

				// Loop through the Sprite List and render all sprites
				for (unsigned int i = 0; i < m_DebugSpriteList.size(); i++)
				{
					Sprite* currentSprite = m_DebugSpriteList[i];
					if (!currentSprite->GetRenderable()) continue;

					// Set the shaders based on the Material of the object being rendered
					{
						currentSprite->GetMaterial()->Set(*s_direct3dDevice);
					}


#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_BeginEvent(0, L"Setting Streams");
#endif

					ID3DXConstantTable* fragmentShaderConstTable = currentSprite->GetMaterial()->GetFragmentShaderConstantTable();
					D3DXHANDLE constantHandle = fragmentShaderConstTable->GetConstantByName(NULL, "g_colorModifier");
					(fragmentShaderConstTable)->SetFloatArray(s_direct3dDevice, constantHandle, currentSprite->GetMaterial()->GetColorModifier(), 3);
					// Bind a specific vertex buffer to the device as a data source
					{
						// There can be multiple streams of data feeding the display adaptor at the same time
						const unsigned int streamIndex = 0;
						// It's possible to start streaming data in the middle of a vertex buffer
						const unsigned int bufferOffset = 0;
						// The "stride" defines how large a single vertex is in the stream of data
						const unsigned int bufferStride = sizeof(sVertex);
						result = s_direct3dDevice->SetStreamSource(streamIndex, currentSprite->GetBufferData(), bufferOffset, bufferStride);
						assert(SUCCEEDED(result));

					}

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
					D3DPERF_EndEvent();
#endif
					// Render objects from the current streams
					{

						const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLESTRIP;
						const unsigned int indexOfFirstVertexToRender = 0;
						const unsigned int primitiveCountToRender = 2;
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
						D3DPERF_BeginEvent(0, L"DebugSprite DrawCall");
#endif
						HRESULT result = s_direct3dDevice->DrawPrimitive(primitiveType, indexOfFirstVertexToRender, primitiveCountToRender);
						assert(SUCCEEDED(result));

#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
						D3DPERF_EndEvent();
#endif
					}
				}
			}
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
			D3DPERF_EndEvent();   // ENDING DEBUG SPRITE RENDERING
#endif
#pragma endregion DrawingDebugSprites
#endif
			// End Scene
			result = s_direct3dDevice->EndScene();
			assert(SUCCEEDED(result));

			// Everything has been drawn to the "back buffer", which is just an image in memory.
			// In order to display it, the contents of the back buffer must be "presented"
			// (to the front buffer)
			{
				const RECT* noSourceRectangle = NULL;
				const RECT* noDestinationRectangle = NULL;
				const HWND useDefaultWindow = NULL;
				const RGNDATA* noDirtyRegion = NULL;
				HRESULT result = s_direct3dDevice->Present(noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion);
				assert(SUCCEEDED(result));
			}
			m_debugMeshList.clear();
			m_debugMeshPositions.clear();
			debugLines.ResetBuffer();
		}
	}
}

// Make sure all devices are released and the Materials are destroyed
bool Engine::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if (s_direct3dInterface)
	{
		if (s_direct3dDevice)
		{
			// Iterate through the Material Map, release their shaders, and delete the Materials
			for (std::map<std::string, Material*>::iterator it = m_MaterialList.begin(); it != m_MaterialList.end(); ++it)
			{
				if (it->second != NULL)
					it->second->ClearMaterial();
			}
			// Clear the Material Map
			m_MaterialList.clear();

			// If in debug mode, release the font
#ifdef DEBUG_MENU_ENABLED
			if (m_font){
				m_font->Release();
				m_font = NULL;
			}
#endif
			for (std::map<std::string, Mesh*>::iterator it = m_MeshList.begin(); it != m_MeshList.end(); ++it)
			{
				if (it->second != NULL)
					it->second->ClearMesh();
			}
			if (s_vertexDeclaration)
			{
				s_direct3dDevice->SetVertexDeclaration(NULL);
				s_vertexDeclaration->Release();
				s_vertexDeclaration = NULL;
			}

			s_direct3dDevice->Release();
			s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_mainWindow = NULL;

	return !wereThereErrors;
}

// Helper Functions
//=================

// Initialize 
//-----------

bool Engine::Graphics::CreateDevice(const HWND i_mainWindow)
{
	const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
	const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
	const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	D3DPRESENT_PARAMETERS presentationParameters = { 0 };
	{
		presentationParameters.BackBufferWidth = eae6320::UserSettings::GetWidth();
		presentationParameters.BackBufferHeight = eae6320::UserSettings::GetHeight();
		presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
		presentationParameters.BackBufferCount = 1;
		presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		presentationParameters.MultiSampleQuality = 3;
		presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentationParameters.hDeviceWindow = i_mainWindow;
		presentationParameters.Windowed = eae6320::UserSettings::IsFullScreenModeEnabled() ? FALSE : TRUE;
		presentationParameters.EnableAutoDepthStencil = TRUE;
		presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		presentationParameters.AutoDepthStencilFormat = D3DFMT_D16;
	}
	HRESULT result = s_direct3dInterface->CreateDevice(useDefaultDevice, useHardwareRendering,
		i_mainWindow, useHardwareVertexProcessing, &presentationParameters, &s_direct3dDevice);
	if (SUCCEEDED(result))
	{
		return true;
	}
	else
	{
		MessageBox(i_mainWindow, "DirectX failed to create a Direct3D9 device", "No D3D9 Device", MB_OK | MB_ICONERROR);
		return false;
	}
}

bool Engine::Graphics::CreateInterface(const HWND i_mainWindow)
{
	// D3D_SDK_VERSION is #defined by the Direct3D header files,
	// and is just a way to make sure that everything is up-to-date
	s_direct3dInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_direct3dInterface)
	{
		return true;
	}
	else
	{
		MessageBox(i_mainWindow, "DirectX failed to create a Direct3D9 interface", "No D3D9 Interface", MB_OK | MB_ICONERROR);
		return false;
	}
}


D3DXMATRIX Engine::Graphics::GetWorldToViewTransform()
{

	D3DXQUATERNION m_Orientation;
	Vector3 CameraPosition = m_Camera.GetPosition();
	m_Orientation.x = CameraPosition.getx();
	m_Orientation.y = CameraPosition.gety();
	m_Orientation.z = CameraPosition.getz();


	D3DXVECTOR3 m_Position;
	Vector3 CameraOrientation = m_Camera.GetOrientation();
	m_Position.x = CameraOrientation.getx();
	m_Position.y = CameraOrientation.gety();
	m_Position.z = CameraOrientation.getz();
	// Calculate the camera's transformation in world space
	D3DXMATRIX transform_viewToWorld;
	{
		D3DXMatrixTransformation(&transform_viewToWorld,
			NULL, NULL, NULL, NULL,
			&m_Orientation, &m_Position);
	}
	// D3DX can calculate the inverse of any matrix:
	//D3DXMATRIX transform_worldToView;
	//D3DXMatrixInverse( &transform_worldToView, NULL, &transform_viewToWorld );
	// But it can be calculated more cheaply since we know a camera can only have
	// rotation and translation:


	// For reference, D3DX also has the following convenience function:

	D3DXMATRIX m_WorldToView(
		transform_viewToWorld._11, transform_viewToWorld._21, transform_viewToWorld._31, 0.0f,
		transform_viewToWorld._12, transform_viewToWorld._22, transform_viewToWorld._32, 0.0f,
		transform_viewToWorld._13, transform_viewToWorld._23, transform_viewToWorld._33, 0.0f,
		-(transform_viewToWorld._41 * transform_viewToWorld._11) - (transform_viewToWorld._42 * transform_viewToWorld._12) - (transform_viewToWorld._43 * transform_viewToWorld._13),
		-(transform_viewToWorld._41 * transform_viewToWorld._21) - (transform_viewToWorld._42 * transform_viewToWorld._22) - (transform_viewToWorld._43 * transform_viewToWorld._23),
		-(transform_viewToWorld._41 * transform_viewToWorld._31) - (transform_viewToWorld._42 * transform_viewToWorld._32) - (transform_viewToWorld._43 * transform_viewToWorld._33),
		1.0f
		);

	D3DXVECTOR3 m_LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_WorldToView, &m_Position, &m_LookAt, &m_Up);
	return m_WorldToView;

}

D3DXMATRIX* Engine::Graphics::GetViewToScreenTransform()
{

	return &m_ViewToScreen;
}

void Engine::Graphics::SetViewToScreen()
{
	{
		const float aspectRatio = static_cast<float>(eae6320::UserSettings::GetWidth()) / static_cast<float>(eae6320::UserSettings::GetHeight());
		D3DXMatrixPerspectiveFovLH(&m_ViewToScreen, D3DXToRadian(60.0f), aspectRatio, 10.0f, 4000.0f);
	}
}

// Load the table containing the names of the Materials to be used in the game
bool Engine::Graphics::LoadMaterialNamesFile(const char* i_path)
{
	bool wereThereErrors = false;

	// Create a new Lua state
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

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);

			std::string errorMessage = std::string("Failed to load Material List");
			MessageBox(s_mainWindow, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
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
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	if (!InitializeMaterialMap(*luaState))
	{
		wereThereErrors = true;
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


// Load the table containing the names of the Materials to be used in the game
bool Engine::Graphics::LoadMeshNamesFile(const char* i_path)
{
	bool wereThereErrors = false;

	// Create a new Lua state
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

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);

			std::string errorMessage = std::string("Failed to load Mesh List");
			MessageBox(s_mainWindow, errorMessage.c_str(), "No Vertex Shader", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
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
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	if (!InitializeMeshMap(*luaState))
	{
		wereThereErrors = true;
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


bool Engine::Graphics::AssignMaterial(const char* i_matName, Sprite* i_Sprite)
{
	std::map<std::string, Material*>::iterator it;
	it = m_MaterialList.find(i_matName);;
	if (it != m_MaterialList.end())
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Material");
#endif
		i_Sprite->SetMaterial(m_MaterialList[i_matName]);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		return true;
	}
	else
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Material");
#endif
		i_Sprite->SetMaterial(m_MaterialList["data/cubeMaterial.txt"]);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		return false;
	}
}


bool Engine::Graphics::AssignMaterial(const char* i_matName, SharedPointer<Actor> i_Actor)
{
	std::map<std::string, Material*>::iterator it;
	it = m_MaterialList.find(i_matName);;
	if (it != m_MaterialList.end())
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Material");
#endif
		i_Actor->SetMaterial(m_MaterialList[i_matName]);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		return true;
	}
	else
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Material");
#endif
		i_Actor->SetMaterial(m_MaterialList["data/cubeMaterial.txt"]);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		return false;
	}
}

bool Engine::Graphics::AssignMesh(const char* i_matName, SharedPointer<Actor> i_Actor)
{
	std::map<std::string, Mesh*>::iterator it;
	it = m_MeshList.find(i_matName);;
	if (it != m_MeshList.end())
	{
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_BeginEvent(0, L"Set Mesh");
#endif
		i_Actor->SetMesh(m_MeshList[i_matName]);
#ifdef EAE6320_GRAPHICS_AREPIXEVENTSENABLED
		D3DPERF_EndEvent();
#endif
		return true;
	}
	else
		return false;
}

bool Engine::Graphics::InitializeMaterialMap(lua_State& io_luaState)
{
	{
		const int key = 1;
		lua_pushinteger(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		const char* value = lua_tostring(&io_luaState, -1);
		if (value)
		{
			std::cout << "The value at key " << key << " is \"" << value << "\"\n";
			lua_pop(&io_luaState, 1);
		}
		else
		{
			std::cerr << "There is no string value at key " << key << "\n";
			lua_pop(&io_luaState, 1);
			return false;
		}
	}

	{

	const int arrayLength = luaL_len(&io_luaState, -1);
	if (arrayLength > 0)
	{
		// Remember that Lua arrays are 1-based and not 0-based!
		for (int i = 1; i <= arrayLength; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			const char* value = lua_tostring(&io_luaState, -1);
			if (value)
			{
				m_MaterialList[value] = new Material();
				bool errors = m_MaterialList[value]->LoadMaterialFromFile(value, s_mainWindow, s_direct3dDevice);
				lua_pop(&io_luaState, 1);
				if (!errors)
				{
					std::cerr << "Failed to load material";
					lua_pop(&io_luaState, 1);
					return false;
				}

			}
			else
			{
				std::cerr << "\tThe value #" << i << " isn't a string!\n";
				lua_pop(&io_luaState, 1);
				return false;
			}
		}
	}
	else
	{
		std::cerr << "The asset table doesn't have any ordered values\n";
		return false;
	}
}
	return true;
}

bool Engine::Graphics::InitializeMeshMap(lua_State& io_luaState)
{
	{
		const int key = 1;
		lua_pushinteger(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		const char* value = lua_tostring(&io_luaState, -1);
		if (value)
		{
			std::cout << "The value at key " << key << " is \"" << value << "\"\n";
			lua_pop(&io_luaState, 1);
		}
		else
		{
			std::cerr << "There is no string value at key " << key << "\n";
			lua_pop(&io_luaState, 1);
			return false;
		}
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
			std::string meshName = std::string("data/Level/") + lua_tostring(&io_luaState, -1) + std::string(".txt");
			const char* value = meshName.c_str();
			if (value)
			{
				m_MeshList[value] = new Mesh();
				bool errors = m_MeshList[value]->LoadMeshDataFromFile(value, s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
				lua_pop(&io_luaState, 1);
				if (!errors)
				{
					std::cerr << "Failed to load Mesh";
					m_MeshList.erase(value);
					lua_pop(&io_luaState, 1);
					return false;
				}
				m_MeshList[value]->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

			}
			else
			{
				std::cerr << "\tThe value #" << i << " isn't a string!\n";
				lua_pop(&io_luaState, 1);
				return false;
			}
		}
	}
	else
	{
		std::cerr << "The asset table doesn't have any ordered values\n";
		return false;
	}
}

	// Populate the map with static meshes in the level

	/*unsigned int totalStaticMeshes = 1000;
	for (unsigned int i = 0; i <= totalStaticMeshes; i++)
	{
	std::string currentMeshName = "data/Level/LevelMesh" + std::to_string(i)+".txt";
	const char* value = currentMeshName.c_str();
	m_MeshList[value] = new Mesh();
	bool errors = m_MeshList[value]->LoadMeshDataFromFile(value, s_mainWindow, s_direct3dDevice, s_vertexDeclaration, s_vertexElements);
	if (!errors)
	{
	std::cerr << "Failed to load Mesh";
	m_MeshList.erase(value);
	return false;
	}
	m_MeshList[value]->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	}*/
	return true;
}


void Engine::Graphics::UpdateLightSource()
{
	if (Engine::UserInput::IsKeyPressed(VK_LCONTROL))
	{
		if (Engine::isApproxZero(g_lightSource.getPosition().getx() - 0.5f))
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx() - 1.0f, g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz()));
		else
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx() - 0.5f, g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz()));

	}
	if (Engine::UserInput::IsKeyPressed(VK_LSHIFT))
	{
		if (Engine::isApproxZero(g_lightSource.getPosition().getx() + 0.5f))
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx() + 1.0f, g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz()));
		else
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx() + 0.5f, g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz()));

	}
	if (Engine::UserInput::IsKeyPressed(VK_RSHIFT))
	{
		if (Engine::isApproxZero(g_lightSource.getPosition().gety() + 0.5f))
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx(), g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz() + 1.0f));
		else
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx(), g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz() + 0.5f));

	}
	if (Engine::UserInput::IsKeyPressed(VK_RCONTROL))
	{
		if (Engine::isApproxZero(g_lightSource.getPosition().gety() - 0.5f))
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx(), g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz() - 1.0f));
		else
			g_lightSource.setPosition(Vector3(g_lightSource.getPosition().getx(), g_lightSource.getPosition().gety(), g_lightSource.getPosition().getz() - 0.5f));

	}
	m_Camera.SetWorldToView();
}

void Engine::Graphics::SetAmbientLight(Vector3 i_lightColor)
{
	g_ambientLight.setColor(i_lightColor);
}

void Engine::Graphics::SetDiffuseLight(Vector3 i_lightColor)
{
	g_lightSource.setColor(i_lightColor);
}

