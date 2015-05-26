#pragma once
#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "World.h"
#include "NamedBitSet.h"
#include "CollisionSystem.h"
#include "StaticObjectCollisionSystem.h"
#include "SharedPointer.h"
#include "DebugMenu.h"
#include "Timer.h"
#include <vector>
#include <map>
#include"AudioSystem.h"
#include"Sound2D.h"
#include "Sound3D.h"
#include"Music.h"


class Game
{

	// Handles to the Window, Program
	HWND m_MainWindow;
	HINSTANCE m_ProgramInstance;

	// Message returned by the window
	MSG m_Message;

	// The reason for the exit
	int m_ExitCode;

	// Instance of Graphics class to handle rendering
	Engine::Graphics* m_Graphics;

	//Instance of World
	Engine::World* gameWorld;

	// Did Graphics initialize correctly
	bool m_GraphicsInitializedCorrectly;

	// Did Collisions initialize correctly
	bool m_collisionSystemInitializedCorrectly;

	// Map of controllers

	// Collisions bit set
	Engine::NamedBitSet<uint32_t> m_CollisionBitSet;

	// FPS Timer
	Engine::Timer m_fpsTimer;
	// Debug Menu
	DebugMenu* m_debugMenu;

	// Static objects collision system
	Engine::StaticObjectCollisionSystem* m_staticsCollisionSystem;
	// Camera Speed
	float m_cameraSpeed;
	
	// Player Position when Fly cam is on
	Engine::Vector3 m_playerDebugPosition;
	Engine::Vector3 m_playerDebugRotation;

	Engine::Sprite* m_sprintBar;
	float m_sprintPower;



	float m_disableInputTimer;
	float m_timer;
	float m_effectsVolume;
	float m_musicVolume;
	bool m_enableFlyCam;
	bool m_displaySphere;
	char m_FPS[10];
	AudioSystem * m_AudioSystem;
	bool m_sprinting;
	bool m_sprintSoundPlaying;
	bool m_sprintOtherPlaying;
	Sound3D* SprintSound;
	Sound3D* OtherPlayerSprintSound;
	Sound3D* FootStepSurface1;
	Sound3D* FootStepSurface2;
	Sound3D* AmbientSound;
	bool m_alreadyPlaying1;
	bool m_alreadyPlaying2;
	bool m_alreadyPlaying3;
public:
	static unsigned int m_teamID;
	static bool m_carryingFlag;
	static int m_scores[2];
	static char* m_scoreText[2];
	static Sound2D* m_flagPickupSounds[2];
	static Sound2D* IPickedUpFlag;
	static Sound2D* TheyPickedUpFlag;
	static Sound2D* IScored;
	static Sound2D* TheyScored;
	static Sound2D* FlagsReset;
	static Music* backGroundSound;


	static Engine::SharedPointer<Engine::Actor> m_flags[2];
	static std::map<std::string, Engine::IActorController*> m_Controllers;
	static std::vector<Engine::SharedPointer<Engine::Actor>> m_CollidedTileList;
	// Player One
	static Engine::SharedPointer<Engine::Actor> m_playerOne;
	static Engine::SharedPointer<Engine::Actor> m_playerTwo;
	// Camera Actor
	static Engine::SharedPointer<Engine::Actor> m_cameraActor;
	Game();
	bool LoadSpriteInfo();
	bool ReadSceneFile(Engine::NamedBitSet<uint32_t> i_CollisionBitSet);
	void CreateHexGrid(int i_numberOfHeightElements, int i_numberOfWidthElements, Engine::NamedBitSet<uint32_t> i_CollisionBitSet);
	void InitializeGame(HINSTANCE, int);
	void InitializeCollisionBitset(Engine::NamedBitSet<uint32_t> &i_CollisionBitSet);
	void SetupPlayer();
	void SetupSounds();
	void SetupDebugMenus();
	void UpdateGame();
	void HandleInputs(Engine::StaticCollisionInfo i_info);
	void DrawGUI();
	void SetupGUI();
	void SetupBaseAreaTriggers();
	int ShutdownGame();

};

#endif