#include "precompiled.h"
#include "Actor.h"
#include "Graphics.h"
#include "World.h"
#include "WindowsProgram.h"
#include "Game.h"
#include "UserController.h"
#include "IActorController.h"
#include "NullController.h"
#include "SharedPointer.h"
#include <assert.h>
#include "LuaHelpers.h"
#include "Sprite.h"
#include "LineRenderer.h"
#include "CollisionSystem.h"
#include "NamedBitSet.h"
#include "UserInput.h"
#include "PhysicsController.h"
#include "CameraController.h"
#include "FollowController.h"
#include "World.h"
#include "DebugMenu.h"
#include "Timer.h"
#include "DebugLog.h"
#include "CollisionOctree.h"
#include "TriggerVolume.h"
#include "HashedString.h"
#include "Networking.h"
void ResetFlags();

int Game::m_scores[2];
char* Game::m_scoreText[2];
bool Game::m_carryingFlag = false;
unsigned int Game::m_teamID;
unsigned int previousFrameScore=0;

Sound2D*  Game::IPickedUpFlag;
Sound2D*  Game::TheyPickedUpFlag;
Sound2D*  Game::IScored;
Sound2D*  Game::TheyScored;
Sound2D*  Game::FlagsReset;
Music*  Game::backGroundSound;
Sound2D* Game::m_flagPickupSounds[2];
std::vector<Engine::SharedPointer<Engine::Actor>> Game::m_CollidedTileList;
Engine::SharedPointer<Engine::Actor> Game::m_playerOne=new Engine::Actor;
Engine::SharedPointer<Engine::Actor> Game::m_playerTwo = new Engine::Actor;
Engine::SharedPointer<Engine::Actor> Game::m_cameraActor = new Engine::Actor;
std::map<std::string, Engine::IActorController*> Game::m_Controllers;
Engine::SharedPointer<Engine::Actor> Game::m_flags[2]{new Engine::Actor, new Engine::Actor};
char* startText = "Press '9' to start as server, '0' as client";
Game::Game() : m_cameraSpeed(10.0f), m_enableFlyCam(false), m_displaySphere(true)
{
	m_timer = 0.0f;
	m_playerOne->setPosition(Engine::Vector3(85.0f, -50.0f, 85.0f));
	m_playerOne->setMaxVelocity(300);
	m_playerTwo->setPosition(Engine::Vector3(10000.0f, -50.0f, 85.0f));
	m_playerTwo->setMaxVelocity(300);
	m_cameraActor->setPosition(Engine::Vector3(0.0f, 0.0f, 0.0f));
	m_cameraActor->setMaxVelocity(300.0f); 
	m_scores[0] = 0;
	m_scores[1] = 0;
	m_scoreText[0] = new char[25];
	strcpy_s(m_scoreText[0],25,"Team 1 Score: 0");
	m_scoreText[1] = new char[25];
	strcpy_s(m_scoreText[1], 25, "Team 2 Score: 0");
	m_sprintPower = 100.0f;
	m_sprintSoundPlaying = false;
	m_sprinting=false;
	m_disableInputTimer = 0.5f;
	m_GraphicsInitializedCorrectly = false;
}


void Game::SetupSounds()
{
	IPickedUpFlag = new Sound2D(m_AudioSystem);
	IPickedUpFlag->Load("data/welcome-bonus.wav");
	TheyPickedUpFlag = new Sound2D(m_AudioSystem);
	TheyPickedUpFlag->Load("data/buzzer-variants-3.wav");
	IScored = new Sound2D(m_AudioSystem);
	IScored->Load("data/winn-up.wav");
	TheyScored = new Sound2D(m_AudioSystem);
	TheyScored->Load("data/nuclear-alarm.wav");
	FlagsReset = new Sound2D(m_AudioSystem);
	FlagsReset->Load("data/jackpot8.wav");

	SprintSound = new Sound3D(m_AudioSystem);
	SprintSound->Load("data/heavy-breathing-off-mic-loop.wav");
	OtherPlayerSprintSound = new Sound3D(m_AudioSystem);
	OtherPlayerSprintSound->Load("data/heavy-breathing-off-mic-loop.wav");
	FootStepSurface1 = new Sound3D(m_AudioSystem);
	FootStepSurface1->Load("data/running-gravel-or-dry-leaves-loop.wav");

	FootStepSurface2 = new Sound3D(m_AudioSystem);
	FootStepSurface2->Load("data/running-hard-surface.wav");
	FootStepSurface1->SetVolume(0.1f);
	FootStepSurface2->SetVolume(0.1f);

	m_flagPickupSounds[0] = IPickedUpFlag;
	m_flagPickupSounds[1] = TheyPickedUpFlag;

	AmbientSound = new Sound3D(m_AudioSystem);
	AmbientSound->Load("data/ambient-sound.wav");
	AmbientSound->SetPosition(0.0f, -1.5f, 0.0f);
	AmbientSound->SetLoop(true);
	AmbientSound->SetVolume(0.5f);
	//ambientSound.SetMinMaxDistance(1.1f, 0.0f);
	AmbientSound->Play();

	backGroundSound = new Music(m_AudioSystem);
	backGroundSound->Load("data/background.wav");
	backGroundSound->SetLoop(true);
	backGroundSound->SetVolume(0.015f);
	m_effectsVolume = 0.02f;
	m_musicVolume = 0.015f;
	backGroundSound->Play();
}

bool CheckPlayerDistances()
{
	if (abs(Game::m_playerOne->getPositionx() - Game::m_playerTwo->getPositionx()) > 100.0f) return false;
	if (abs(Game::m_playerOne->getPositiony() - Game::m_playerTwo->getPositiony()) > 100.0f) return false;
	if (abs(Game::m_playerOne->getPositionz() - Game::m_playerTwo->getPositionz()) > 100.0f) return false;
	return true;
}
void SetRectInScreenCoordinates(Engine::Quad i_quad, RECT* o_rect)
{
	Engine::NativeResolution resolution = Engine::Sprite::GetNativeResolution();
	float left = (1 + i_quad.getTopLeftX())*eae6320::UserSettings::GetWidth() / 2;
	float top = (1 - i_quad.getTopLeftY())*eae6320::UserSettings::GetHeight() / 2;
	float right = left + i_quad.getWidth()*eae6320::UserSettings::GetWidth() / 2;;
	float bottom = top + i_quad.getHeight()*eae6320::UserSettings::GetHeight() / 2;;
	SetRect(o_rect, (int)left, (int)top, (int)right, (int)bottom);
}
void drawForwardRay()
{
	Engine::Graphics* graphicsObject = Engine::Graphics::GetInstance();
	std::vector<Engine::AABB> collidedBoxes;
	std::vector<D3DCOLOR> colors;
	graphicsObject->ClearPersistentDebugLines();
	graphicsObject->AddPersistentDebugLine(Game::m_playerOne->getPosition(), Game::m_playerOne->getPosition() + Game::m_playerOne->getForward()*300.0f, D3DCOLOR_ARGB(255, 0, 0, 255));

	Engine::StaticObjectCollisionSystem::GetInstance()->GetCollidedAABBs(Game::m_playerOne->getPosition(), Game::m_playerOne->getPosition() + Game::m_playerOne->getForward()*300.0f, &collidedBoxes, &colors);
	for (unsigned int i = 0; i < collidedBoxes.size(); i++)
	{
		float halfWidth = collidedBoxes[i].getHalfWidth();
		/*graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, -halfWidth), colors[i]);
*/
		//Front Cube
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, -halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, -halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, -halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, -halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, -halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, -halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, -halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, -halfWidth), colors[i]);

		//Back Cube
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), colors[i]);


		//Side Cubes
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, halfWidth, -halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(-halfWidth, -halfWidth, -halfWidth), colors[i]);

		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, halfWidth, -halfWidth), colors[i]);
		graphicsObject->AddPersistentDebugLine(collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, halfWidth), collidedBoxes[i].getCenter() + Engine::Vector3(halfWidth, -halfWidth, -halfWidth), colors[i]);

		//graphicsObject->AddDebugCube(collidedBoxes[i].getHalfHeight() * 2, collidedBoxes[i].getHalfHeight() * 2, collidedBoxes[i].getHalfHeight() * 2, collidedBoxes[i].getCenter(), D3DCOLOR_ARGB(255, 255, 255, 255));//colors[i]);
	}

}

bool m_shot;
bool g_player1Turn = false;
int redScore = 0;
void destroyOnCollision(Engine::SharedPointer<Engine::Actor> i_SharedPtr, Engine::CollisionInformation i_Info)
{
	i_SharedPtr->setMarkedForDeath(true);
	m_shot = false;
}


void Game::InitializeGame(HINSTANCE i_thisInstanceOfTheProgram, int i_initialWindowDisplayState)
{

	// Create a Window and Initialize the Game member variables 
	bool windowSuccessfullyCreated = CreateMainWindow(i_thisInstanceOfTheProgram, i_initialWindowDisplayState);
	assert(windowSuccessfullyCreated);      // Make sure window creation is successful
	m_ProgramInstance = i_thisInstanceOfTheProgram;
	m_MainWindow = GetMainWindow();

	// Setup Graphics
	m_Graphics = Engine::Graphics::GetInstance();
	assert(m_Graphics != NULL);                       // Make sure we have a valid pointer to the Graphics class instance
	m_GraphicsInitializedCorrectly = m_Graphics->Initialize(m_MainWindow);
	assert(m_GraphicsInitializedCorrectly);          // Make sure graphics are initialized

	// Setup Collisions
	m_staticsCollisionSystem = Engine::StaticObjectCollisionSystem::GetInstance();
	assert(m_staticsCollisionSystem != NULL);                       // Make sure we have a valid pointer to the Graphics class instance
	m_collisionSystemInitializedCorrectly = m_staticsCollisionSystem->Initialize(m_MainWindow);
	assert(m_collisionSystemInitializedCorrectly);

	
	// Initialize the Controller
	Engine::IActorController* userController=new UserController();
	Engine::IActorController* nullController(new NullController());
	Engine::IActorController* physicsController(new PhysicsController());
	Engine::IActorController* cameraController(new CameraController(m_playerOne));
	Engine::IActorController* followController(new FollowController(m_playerOne));
	Engine::IActorController* followControllerEnemy(new FollowController(m_playerTwo));

	m_Controllers["NullController"] = nullController;
	m_Controllers["UserController"] = userController;
	m_Controllers["PhysicsController"] = physicsController;
	m_Controllers["CameraController"] = cameraController;
	m_Controllers["FollowController"] = followController;
	m_Controllers["FollowControllerEnemy"] = followControllerEnemy;
	// Setup Collision Info
	InitializeCollisionBitset(m_CollisionBitSet);
	m_Graphics->SetDebugLinesCapacity(5000);
	// Create Hex Grid
	//CreateHexGrid(15, 15,m_CollisionBitSet);

	// Load Audio System
	m_AudioSystem = AudioSystem::GetInstance();
	// Load Scene
	bool m_sceneLoadedCorrectly = false;
	m_sceneLoadedCorrectly=ReadSceneFile(m_CollisionBitSet);
	assert(m_sceneLoadedCorrectly);

	// Setup Player and Camera
	m_playerOne->addController(m_Controllers["PhysicsController"]);
	m_playerOne->addController(m_Controllers["UserController"]);
	m_playerTwo->addController(m_Controllers["NullController"]);
	m_cameraActor->addController(m_Controllers["CameraController"]);


	m_Graphics->AssignMesh("data/Level/soccerBallMesh.txt", m_playerOne);
	m_Graphics->AssignMaterial("data/soccerBallMaterial.txt", m_playerOne);

	m_Graphics->AssignMesh("data/Level/soccerBallMesh.txt", m_playerTwo);
	m_Graphics->AssignMaterial("data/soccerBallMaterial.txt", m_playerTwo);
	//Engine::World::addActor(m_playerOne);
	//Engine::World::addActor(m_cameraActor);
	m_playerOne->SetIsRenderable(false);
	Engine::World::addRenderable(m_playerOne);
	Engine::World::addRenderable(m_playerTwo);
	bool m_spritesLoadedCorrectly = false;
	m_spritesLoadedCorrectly = LoadSpriteInfo();
	assert(m_spritesLoadedCorrectly);
	SetupBaseAreaTriggers();
	SetupDebugMenus();
	//SetupGUI();
	m_shot = false;
	

	// Setup initial text
	Engine::Quad textQuad(-1.0f, 1.0f, 0.2f, 0.2f);
	RECT textRect;
	SetRectInScreenCoordinates(textQuad, &textRect);
	m_Graphics->addGUIText(startText, textRect);
	SetupSounds();
}

void Game::InitializeCollisionBitset(Engine::NamedBitSet<uint32_t> &i_CollisionBitSet)
{

	// Initialize all types in the bitset
	uint32_t tileBit = i_CollisionBitSet.getBitIndex("Tile");
	uint32_t projectileBit = i_CollisionBitSet.getBitIndex("Projectile");


	i_CollisionBitSet.setCollisionMask(tileBit, i_CollisionBitSet.getBitMask("Projectile"));
	i_CollisionBitSet.setCollisionMask(projectileBit, i_CollisionBitSet.getBitMask("Tile"));
}
void GetCursorPosition(int& o_xValue, int& o_yValue)
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	o_xValue = mousePoint.x;
	o_yValue = mousePoint.y;
}


void CopyString(const char* i_inputString, char* o_outputString)
{
	// The user is responsible for ensuring that the output buffer is large enough
	unsigned int currentElement = 0;
	while (i_inputString[currentElement] != '\0')
	{
		o_outputString[currentElement]
			= i_inputString[currentElement];

		++currentElement;
	}
	o_outputString[currentElement] = '\0';
}

void Game::UpdateGame()
{
	if (!m_GraphicsInitializedCorrectly) return;
	// Update Window and Render each frame, till the user decides to quit
	do{
		m_fpsTimer.UpdateTime();
		backGroundSound->SetVolume(m_musicVolume);
		IPickedUpFlag->SetVolume((m_effectsVolume)*0.5f);
		TheyPickedUpFlag->SetVolume(m_effectsVolume);
		IScored->SetVolume(m_effectsVolume);
		TheyScored->SetVolume(m_effectsVolume);
		FlagsReset->SetVolume(m_effectsVolume);
		//drawForwardRay();
#ifdef _DEBUG
		// Draw Debug Shapes
		//if (m_displaySphere)
		//	m_Graphics->AddDebugSphere(40.0f, Engine::Vector3(0.0f, 0.0f, 400.0f));
		// If fly cam button toggled, store players position
		
		//if (m_enableFlyCam)
		//	m_Graphics->AddDebugCube(100.0f, 200.0f, 100.0f, m_playerOne->getPosition());
		
		m_Graphics->AddDebugLine(m_playerOne->getPosition(), m_playerOne->getPosition() + Engine::Vector3(m_playerOne->getForward().getx(), 0.0f, m_playerOne->getForward().getz())*100.0f);
#endif
		sprintf_s(m_FPS, 10, "%.2f", 1/m_fpsTimer.DeltaTime_ms() );
		// BCheck for Bcollisions
		//Engine::CollisionSystem::checkForCollisions(0.016f);
		//Engine::World::checkForDeath();
		//Engine::CollisionSystem::checkForDeath();
		UpdateWindow(m_Message);

	
		Engine::Vector3 m_playerPosition = m_playerOne->getPosition();

		Engine::Vector3 m_playerForward = m_playerOne->getForward();
		Engine::StaticCollisionInfo currentFrameCollisionInfo = m_staticsCollisionSystem->CheckForCollision(m_playerPosition, Engine::Vector3(100.0f*m_playerForward.getx(), 100.0f*m_playerForward.gety(), 100.0f*m_playerForward.getz()));
		m_playerOne->setPosition(m_playerPosition);

		CameraController* camCont = dynamic_cast<CameraController*>(m_Controllers["CameraController"]);

		if (!currentFrameCollisionInfo.m_backwardX && !currentFrameCollisionInfo.m_backwardZ && !currentFrameCollisionInfo.m_camCollided)
		{
			camCont->setDispacement(camCont->getDisplacement()+40.0f);
		}
		if (currentFrameCollisionInfo.m_camCollided)
		{
			float displacement = currentFrameCollisionInfo.camDistance;
			float difference = displacement - camCont->getDisplacement();
			if (abs(difference)>50.0f)
			{
				camCont->setDispacement(camCont->getDisplacement() + 40.0f*(difference / abs(difference)));
			}
		};
		if (!currentFrameCollisionInfo.m_groundCheck)
			m_playerOne->setVelocityy(m_playerOne->getVelocityy() - 30.0f);
		else
			m_playerOne->setVelocityy(0.0f);

		float XdistanceFromPlayer = currentFrameCollisionInfo.backwardXdistance;
		float ZdistanceFromPlayer = currentFrameCollisionInfo.backwardZdistance;

		if (currentFrameCollisionInfo.m_forwardX)
		{

			if (m_playerOne->getVelocityx()*m_playerOne->getForward().getx() >= 0.0f)
				m_playerOne->setVelocityx(0.0f);


		}
		if (currentFrameCollisionInfo.m_backwardX)
		{
			if (XdistanceFromPlayer < 30.0f)
			{
				if (m_playerOne->getVelocityx()*m_playerOne->getForward().getx() < 0.0f)
					m_playerOne->setVelocityx(0.0f);
			}

			CameraController* camCont=dynamic_cast<CameraController*>(m_Controllers["CameraController"]);
			float displacement = currentFrameCollisionInfo.backwardXdistance;
			float difference = displacement - camCont->getDisplacement();
			if (abs(difference)>50.0f)
			{
				camCont->setDispacement(camCont->getDisplacement() + 40.0f*(difference / abs(difference)));
			}
			//m_cameraActor->setPositionx(currentFrameCollisionInfo.backwardXdistance);
		}
		if (currentFrameCollisionInfo.m_forwardZ)
		{
			if (m_playerOne->getVelocityz()*m_playerOne->getForward().getz() >= 0.0f)
				m_playerOne->setVelocityz(0.0f);


		}

		if (currentFrameCollisionInfo.m_backwardZ)
		{
			if (ZdistanceFromPlayer < 30.0f)
			{
				if (m_playerOne->getVelocityz()*m_playerOne->getForward().getz() < 0.0f)
					m_playerOne->setVelocityz(0.0f);
			}

			CameraController* camCont = dynamic_cast<CameraController*>(m_Controllers["CameraController"]);
			float displacement =currentFrameCollisionInfo.backwardZdistance
				;
			float difference = displacement-camCont->getDisplacement() ;
			if (abs(difference)>50.0f)
			{
				camCont->setDispacement(camCont->getDisplacement() + 40.0f*(difference / abs(difference)));
			}
			//m_cameraActor->setPositionz(currentFrameCollisionInfo.backwardZdistance);

		}
		if (!Engine::Networking::GetInstance()->isInit())
		{
			if (Engine::UserInput::IsKeyPressed('9'))
			{
				m_playerOne->SetIsRenderable(true);
				Engine::Networking::GetInstance()->initialize(true);
				m_Graphics->clearGUIText();
				SetupGUI();
				m_teamID = 0;
			}
			if (Engine::UserInput::IsKeyPressed('0'))
			{
				m_playerOne->SetIsRenderable(true);
				Engine::Networking::GetInstance()->initialize(false);
				m_Graphics->clearGUIText();
				SetupGUI();
				m_teamID = 1;
			}

		}
		else
		{
			Engine::Networking::GetInstance()->Update();
			Engine::Networking::GetInstance()->selfPlayerData.position = m_playerOne->getPosition();
			Engine::Networking::GetInstance()->selfPlayerData.rotation = m_playerOne->getRotation();
			Engine::Networking::GetInstance()->selfPlayerData.score = m_scores[m_teamID];
			Engine::Networking::GetInstance()->selfPlayerData.flagPicked = m_carryingFlag;
			Engine::Networking::GetInstance()->selfPlayerData.shiftPressed = m_sprinting;
			SprintSound->SetPosition(m_playerOne->getPosition());

		
			if (!m_debugMenu->GetState())
			{
				for (unsigned int i = 0; i < Engine::World::getActorList().size(); i++)
				{
					Engine::SharedPointer<Engine::Actor> currentActor = Engine::World::getActorList()[i];
					currentActor->update(0.016f);
				}
				m_playerOne->update(0.0166f);
				m_cameraActor->update(0.0166f);
				CameraController* camCont = dynamic_cast<CameraController*>(m_Controllers["CameraController"]);

				// Update other player
				Game::m_playerTwo->setPosition(Engine::Vector3(Engine::Networking::GetInstance()->otherPlayerData.position));
				Game::m_playerTwo->setRotation(Engine::Vector3(Engine::Networking::GetInstance()->otherPlayerData.rotation));
				OtherPlayerSprintSound->SetPosition(m_playerTwo->getPosition());

			
				if (m_teamID == 0)
				{
					Engine::Networking::GetInstance()->selfPlayerData.flagPos = m_flags[1]->getPosition();
					m_flags[0]->setPosition(Engine::Networking::GetInstance()->otherPlayerData.flagPos);
				}
				else
				{
					Engine::Networking::GetInstance()->selfPlayerData.flagPos = m_flags[0]->getPosition();
					m_flags[1]->setPosition(Engine::Networking::GetInstance()->otherPlayerData.flagPos);
				}

				if (Game::m_teamID == 0)
				{
					char buf[25];
					strcpy_s(buf, 25, "Team 2 Score: ");
					strcat_s(buf, 25, std::to_string(Engine::Networking::GetInstance()->otherPlayerData.score).c_str());
					strcpy_s(Game::m_scoreText[1], 25, buf);
				}
				if (Game::m_teamID == 1)
				{
					char buf[25];
					strcpy_s(buf, 25, "Team 1 Score: ");
					strcat_s(buf, 25, std::to_string(Engine::Networking::GetInstance()->otherPlayerData.score).c_str());
					strcpy_s(Game::m_scoreText[0], 25, buf);
				}
				DrawGUI();
			}
		}
		
		m_playerOne->setMaxVelocity(300.0f);
		m_sprintPower += 0.1f;
		m_sprinting = false;
		if (m_sprintPower > 100.0f)
			m_sprintPower = 100.0f;
		if (m_teamID == 0)
		{
			Engine::TriggerVolume::CheckIfIntersecting(m_playerOne->getPosition(), Engine::HashedString("Flag2"));
			Engine::TriggerVolume::CheckIfIntersecting(m_playerOne->getPosition(), Engine::HashedString("Base1"));
		}
		else
		{
			Engine::TriggerVolume::CheckIfIntersecting(m_playerOne->getPosition(), Engine::HashedString("Flag1"));
			Engine::TriggerVolume::CheckIfIntersecting(m_playerOne->getPosition(), Engine::HashedString("Base2"));
		}
		HandleInputs(currentFrameCollisionInfo);
		if (m_sprinting && !m_sprintSoundPlaying)
		{
			m_sprintSoundPlaying = true;
			SprintSound->SetLoop(true);
			SprintSound->Play();
		}
		if (!m_sprinting && m_sprintSoundPlaying)
		{
			m_sprintSoundPlaying = false;
			SprintSound->SetLoop(false);

			SprintSound->Pause();
		}

		if (Engine::Networking::GetInstance()->otherPlayerData.shiftPressed && !m_sprintOtherPlaying)
		{
			m_sprintOtherPlaying = true;
			OtherPlayerSprintSound->SetLoop(true);
			OtherPlayerSprintSound->Play();
		}
		if (!Engine::Networking::GetInstance()->otherPlayerData.shiftPressed)
		{
			m_sprintOtherPlaying = false;
			OtherPlayerSprintSound->SetLoop(false);
			OtherPlayerSprintSound->Pause();
		}
		// Check if players colliding with each other
		if (CheckPlayerDistances())
		{
			ResetFlags();
		}
		m_Graphics->Render();
	} while (m_Message.message != WM_QUIT);

	m_ExitCode = static_cast<int>(m_Message.wParam);
}



void Game::SetupGUI()
{
	// Draw Text
	Engine::Quad textQuad(-1.0f, 1.0f, 0.2f, 0.2f);
	RECT textRect;
	SetRectInScreenCoordinates(textQuad, &textRect);
	m_Graphics->addGUIText(m_scoreText[0], textRect);

	Engine::Quad textQuad1(-1.0f, 0.8f, 0.2f, 0.2f);
	RECT textRect1;
	SetRectInScreenCoordinates(textQuad1, &textRect1);
	m_Graphics->addGUIText(m_scoreText[1], textRect1);

	
	// Setup Sprint Bar
	m_sprintBar = new Engine::Sprite;
	m_sprintBar->SetQuad(Engine::Quad(-0.5f, 1.0f, 1.0f, 0.2f));
	m_Graphics->AssignMaterial("data/spriteMaterial_RedBar.txt", m_sprintBar);
	m_Graphics->addSprite(m_sprintBar);

}
void Game::DrawGUI()
{
	m_sprintBar->SetQuad(Engine::Quad(-0.5f, 1.0f, 1.0f*m_sprintPower/100.0f, 0.2f));
	m_sprintBar->FillQuadMeshBuffer(m_MainWindow);
}

void callbackFunction()
{
	Engine::Graphics::GetInstance()->GetCamera()->SetPosition(Engine::Vector3(0.0f, 0.0f, 0.0f));
	Engine::Graphics::GetInstance()->GetCamera()->SetOrientation(Engine::Vector3(0.0f, 0.0f, 0.0f));
}

void cameraAttachmentFunction(bool i_value, void* i_Actor)
{
	if (i_value)
	{
		Game::m_playerOne->removeController();
		Game::m_playerOne->addController(Game::m_Controllers["NullController"]);
		Game::m_cameraActor->removeController();
		Game::m_cameraActor->addController(Game::m_Controllers["PhysicsController"]);
		Game::m_cameraActor->addController(Game::m_Controllers["UserController"]);
	}
	else
	{
		Game::m_playerOne->removeController();
		Game::m_playerOne->addController(Game::m_Controllers["PhysicsController"]);
		Game::m_playerOne->addController(Game::m_Controllers["UserController"]);
		Game::m_cameraActor->removeController();
		Game::m_cameraActor->addController(Game::m_Controllers["CameraController"]);
	}

}

void Game::HandleInputs(Engine::StaticCollisionInfo i_info)
{
	
	Engine::Camera& camera = *Engine::Graphics::GetInstance()->GetCamera();
	if (!m_debugMenu->GetState())
	{
		if (Engine::UserInput::IsKeyPressed(VK_SPACE))
		{
			if (m_sprintPower >= 0.0f)
			{
				m_playerOne->setMaxVelocity(700.0f);
				m_sprinting = true;
				//m_sprintPower -= 0.5f;
				if (m_sprintPower < 0.0f)
				{
					m_sprintPower = -5.0f;
				}
			}
			
		}
		else
		{
			m_sprinting = false;
		}

		if (Engine::UserInput::IsKeyPressed(VK_NUMPAD1))
		{
			if (!m_alreadyPlaying1)
			{
				FootStepSurface2->SetLoop(false);
				FootStepSurface2->Pause();
				
				m_alreadyPlaying1 = true;
				FootStepSurface1->SetLoop(true);
				FootStepSurface1->Play();
				
				
				m_alreadyPlaying2 = false;
				m_alreadyPlaying3 = false;
			}
		}
		if (Engine::UserInput::IsKeyPressed(VK_NUMPAD2))
		{
			if (!m_alreadyPlaying2)
			{
				FootStepSurface1->SetLoop(false);
				FootStepSurface1->Pause();
				FootStepSurface2->SetLoop(true);
				FootStepSurface2->Play();
				
				m_alreadyPlaying2 = true;
				m_alreadyPlaying1 = false;
				m_alreadyPlaying3 = false;

			}
		}

		if (Engine::UserInput::IsKeyPressed(VK_NUMPAD3))
		{
			if (!m_alreadyPlaying3)
			{
				FootStepSurface1->SetLoop(false);
				FootStepSurface1->Pause();
				FootStepSurface2->SetLoop(false);

				FootStepSurface2->Pause();
				m_alreadyPlaying2 = false;
				m_alreadyPlaying1 = false;
				m_alreadyPlaying3 = true;
			}
		}
		/*
		if (Engine::UserInput::IsKeyPressed(VK_NUMPAD6))
		{
			camera.SetOrientation(Engine::Vector3(camera.GetOrientation().getx(), camera.GetOrientation().gety(), camera.GetOrientation().getz() + 0.03f));
		}
		
		if (Engine::UserInput::IsKeyPressed(VK_NUMPAD4))
		{
			camera.SetOrientation(Engine::Vector3(camera.GetOrientation().getx(), camera.GetOrientation().gety(), camera.GetOrientation().getz() - 0.03f));
		}*/

	}
	camera.SetPosition(m_cameraActor->getPosition());
	camera.SetOrientation(m_cameraActor->getRotation());
	camera.SetWorldToView();

	m_timer++;
	if (m_timer <= m_disableInputTimer) return;

	m_timer = 0.0f;
	if (m_debugMenu->GetState())
	{
		if (Engine::UserInput::IsKeyPressed(VK_UP))
		{
			m_debugMenu->Update(EUp, m_MainWindow);
		}
		if (Engine::UserInput::IsKeyPressed(VK_DOWN))
		{
			m_debugMenu->Update(EDown, m_MainWindow);
		}
		if (Engine::UserInput::IsKeyPressed(VK_LEFT))
		{
			m_debugMenu->Update(ELeft, m_MainWindow);
		}
		if (Engine::UserInput::IsKeyPressed(VK_RIGHT))
		{
			m_debugMenu->Update(ERight, m_MainWindow);
		}
	}
#ifdef DEBUG_MENU_ENABLED
	if (Engine::UserInput::IsKeyPressed(VK_OEM_3))
	{
		m_debugMenu->Activate(!m_debugMenu->GetState());
		Engine::Graphics::GetInstance()->SetDebugMenuState(m_debugMenu->GetState());
	}

#endif
}

void DoNothing(bool, void*)
{

}
void Game::SetupDebugMenus()
{
	m_debugMenu = new DebugMenu(-0.5f, 0.8f, 0.5f, 0.2f);
	m_debugMenu->CreateSlider("CameraSpeed",&m_cameraSpeed,5.0f,50.0f);
	m_debugMenu->CreateSlider("Effects Volume", &m_effectsVolume, 0.0f, 0.5f);
	m_debugMenu->CreateSlider("Music Volume", &m_musicVolume, 0.0f, 0.5f);
	m_debugMenu->CreateCheckBox("EnableFlyCam", &m_enableFlyCam, cameraAttachmentFunction, (void*)(&m_playerOne));
	//m_debugMenu->CreateCheckBox("DisplaySphere", &m_displaySphere, DoNothing, (void*)(&m_playerOne));
	m_debugMenu->CreateButton("ShowCollidingAABBs", drawForwardRay);
	m_debugMenu->CreateButton("ResetCamera", callbackFunction);
	m_debugMenu->CreateText("FPS", m_FPS);

}

int Game::ShutdownGame()
{

	// Clean up directX related stuff and prepare for end of program
	m_Graphics->ShutDown();
	// Clean up Main Window and report on any errors in the process
	if (!OnMainWindowClosed(m_ProgramInstance))
	{
		if (m_ExitCode)
		{
			// If the program already had errors while waiting for the main window to close
			// the existing error exit code can be returned
			return m_ExitCode;
		}
		else
		{
			// In a real program you might decide to define a special constant for this situation,
			// but for this example program -1 is a good arbitrary error code
			return -1;
		}
	}
	return m_ExitCode;
}
bool Game::LoadSpriteInfo()
{

	bool wereThereErrors = false;
	lua_State* luaState = NULL;
	int stackValues = 0;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			std::cerr << "Failed to create a new Lua state\n";
			goto OnExit;
		}
	}

	// Load the asset file into a table at the top of the stack
	{
		const int luaResult = luaL_dofile(luaState, "data/spriteInformation.txt");
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
			std::string errorMessage = std::string("Failed to load sprite info File ") + "data/spriteInformation.txt";
			MessageBox(m_MainWindow, errorMessage.c_str(), "Failed to load sprite info File", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}

	stackValues++;

	//Load Native Width
	if (!LuaHelpers::LoadLuaNumber(*luaState, "NativeWidth", stackValues))
	{
		goto OnExit;
	}

	int nativeWidth = (int)lua_tonumber(luaState, -1);
	lua_pop(luaState, 1);
	stackValues--;

	//Load Native Height
	if (!LuaHelpers::LoadLuaNumber(*luaState, "NativeHeight", stackValues))
	{
		goto OnExit;
	}

	int nativeHeight = (int)lua_tonumber(luaState, -1);
	lua_pop(luaState, 1);
	stackValues--;

	// Set Native Resolution
	Engine::Sprite::SetNativeResolution(Engine::NativeResolution(nativeWidth, nativeHeight));


	// Load sprites table

	if (!LuaHelpers::LoadLuaTable(*luaState, "Sprites", stackValues))
	{
		goto OnExit;
	}
	stackValues++;
	// Load all the sprites
	lua_pushnil(luaState);
	stackValues++;
	const int tableIndex = -2;
	{
		while (lua_next(luaState, tableIndex))
		{
			//Taking the value pushed by luanext into account
			stackValues++;
			if (lua_type(luaState, -1) != LUA_TTABLE)
			{
				std::cerr << "Value is not a table \n";
				lua_pop(luaState, stackValues);
				return false;
			}
			{

				Engine::Sprite* newSprite = new Engine::Sprite();

				// Load the top left position
				if (!LuaHelpers::LoadLuaTable(*luaState, "TopLeft", stackValues))
				{
					goto OnExit;
				}

				// Store the positions
				Engine::Quad spriteQuad;
				Engine::Vector3 position;
				if (!LuaHelpers::LoadFloat2(*luaState, stackValues, position))
				{
					goto OnExit;
				}
				spriteQuad.setTopLeftX(position.getx());
				spriteQuad.setTopLeftY(position.gety());
				lua_pop(luaState, 1);
				stackValues--;

				// Load the width
				if (!LuaHelpers::LoadLuaNumber(*luaState, "Width", stackValues))
				{
					goto OnExit;
				}

				spriteQuad.setWidth((float)lua_tonumber(luaState, -1));
				lua_pop(luaState, 1);
				stackValues--;

				// Load the height
				if (!LuaHelpers::LoadLuaNumber(*luaState, "Height", stackValues))
				{
					goto OnExit;
				}

				spriteQuad.setHeight((float)lua_tonumber(luaState, -1));
				newSprite->SetQuad(spriteQuad);
				lua_pop(luaState, 1);
				stackValues--;
				

				// Load the material name
				if (!LuaHelpers::LoadLuaString(*luaState, "Material", stackValues))
				{
					goto OnExit;
				}

				// Store the actors material
				std::string spriteMaterial = lua_tostring(luaState, -1);
				spriteMaterial = "data/" + spriteMaterial + ".txt";

				m_Graphics->AssignMaterial(spriteMaterial.c_str(), newSprite);
				lua_pop(luaState, 1);
				stackValues--;
				// Check if sprite is a sprite sheet
				if (!LuaHelpers::LoadLuaBoolean(*luaState, "IsSpriteSheet", stackValues))
				{
					goto OnExit;
				}

				bool isSpriteSheet = false;
				if(lua_toboolean(luaState, -1)!=0)
					isSpriteSheet = true;
				lua_pop(luaState, 1);
				stackValues--;

				// Load sprite sheet data if necessary
				if (isSpriteSheet)
				{
					Engine::SpriteSheetProperties sheetProperties;
					// Load the width
					if (!LuaHelpers::LoadLuaNumber(*luaState, "SheetWidth", stackValues))
					{
						goto OnExit;
					}
					
					sheetProperties.m_widthElements = (int)lua_tonumber(luaState, -1);
					lua_pop(luaState, 1);
					stackValues--;

					// Load the height
					if (!LuaHelpers::LoadLuaNumber(*luaState, "SheetHeight", stackValues))
					{
						goto OnExit;
					}

					sheetProperties.m_heightElements = (int)lua_tonumber(luaState, -1);
					lua_pop(luaState, 1);
					stackValues--;
					sheetProperties.m_currentElement = 0;
					newSprite->SetSpriteSheetProperties(sheetProperties);

				}

				// Adjust dimensions for resolution
				Engine::Sprite:: AdjustDimensionsForResolution(newSprite->GetQuad());
				// Add actor to world
				m_Graphics->addSprite(newSprite);
			}

			// To iterate to the next key/value pair, pop the value, but leave the key:
			lua_pop(luaState, 1);
			stackValues--;
		}
	}
	// Popped by lua next
	stackValues--;
	// Pop the sprites table and the lua file
	lua_pop(luaState, 2);
	stackValues-=2;

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

void AssignControllerToFlag(Engine::SharedPointer<Engine::Actor> i_flag,Engine::TriggerVolume* i_trigger)
{
	Game::IPickedUpFlag->Play();
	//Game::TheyPickedUpFlag->Play();
	i_flag->removeController();
	i_flag->addController(Game::m_Controllers["FollowController"]);
	dynamic_cast<FollowController*>(Game::m_Controllers["FollowController"])->SetOffset(Game::m_playerOne->getPosition());
	if (i_trigger!=nullptr)
		i_trigger->SetActive(false);
	Game::m_carryingFlag = true;

}

void ResetFlags()
{
	if (!Game::m_carryingFlag)	return;
	Game::FlagsReset->Play();
	Game::m_carryingFlag = false;
	for (unsigned int i = 0; i < 2; i++)
	{
		Game::m_flags[i]->setPosition(Engine::Vector3(0.0f, 0.0f, 0.0f));
		Game::m_flags[i]->removeController();

	}
	new Engine::TriggerVolume(Game::m_flags[0], Engine::AABB(Engine::Vector3(-1408.0f, 137.0f, -1453.0f), 50.0f), Engine::HashedString("Flag1"), AssignControllerToFlag);
	new Engine::TriggerVolume(Game::m_flags[1], Engine::AABB(Engine::Vector3(1408.0f, 137.0f, 1453.0f), 50.0f), Engine::HashedString("Flag2"), AssignControllerToFlag);
}

void ResetRound(Engine::SharedPointer<Engine::Actor> i_baseTrigger, Engine::TriggerVolume* i_trigger)
{

	if (!Game::m_carryingFlag)	return;
	Game::m_scores[Game::m_teamID]++;
	Game::m_carryingFlag = false;
	Game::IScored->Play();
	//Game::TheyScored->Play();
	for (unsigned int i = 0; i < 2; i++)
	{
		Game::m_flags[i]->setPosition(Engine::Vector3(0.0f, 0.0f, 0.0f));
		Game::m_flags[i]->removeController();
		
	}
	new Engine::TriggerVolume(Game::m_flags[0], Engine::AABB(Engine::Vector3(-1408.0f, 137.0f, -1453.0f), -50.0f), Engine::HashedString("Flag1"), AssignControllerToFlag);
	new Engine::TriggerVolume(Game::m_flags[1], Engine::AABB(Engine::Vector3(1408.0f, 137.0f, 1453.0f), 50.0f), Engine::HashedString("Flag2"), AssignControllerToFlag);

	if (Game::m_teamID == 0)
	{
		char buf[25];
		strcpy_s(buf, 25, "Team 1 Score: ");
		strcat_s(buf, 25, std::to_string(Game::m_scores[0]).c_str());
		strcpy_s(Game::m_scoreText[0], 25, buf);
	}
	if (Game::m_teamID == 1)
	{
		char buf[25];
		strcpy_s(buf, 25, "Team 2 Score: ");
		strcat_s(buf, 25, std::to_string(Game::m_scores[1]).c_str());
		strcpy_s(Game::m_scoreText[1], 25, buf);
	}
}

void Game::SetupBaseAreaTriggers()
{
	Engine::SharedPointer<Engine::Actor> m_baseTrigger1(new Engine::Actor);
	new Engine::TriggerVolume(m_baseTrigger1, Engine::AABB(Engine::Vector3(-1408.0f, 137.0f, -1453.0f), 300.0f), Engine::HashedString("Base1"), ResetRound);

	Engine::SharedPointer<Engine::Actor> m_baseTrigger2(new Engine::Actor);
	new Engine::TriggerVolume(m_baseTrigger2, Engine::AABB(Engine::Vector3(1408.0f, 137.0f, 1453.0f), 300.0f), Engine::HashedString("Base2"), ResetRound);
	//new Engine::TriggerVolume(m_baseTrigger, Engine::AABB(Engine::Vector3(0.0f,0.0f,0.0f), 300.0f), Engine::HashedString("Base"), ResetRound);

}


bool Game::ReadSceneFile(Engine::NamedBitSet<uint32_t> i_CollisionBitSet)
{
	bool wereThereErrors = false;
	lua_State* luaState = NULL;
	int stackValues = 0;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			std::cerr << "Failed to create a new Lua state\n";
			goto OnExit;
		}
	}

	// Load the asset file into a table at the top of the stack
	{
		const int luaResult = luaL_dofile(luaState, "data/sceneFile.txt");
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
			std::string errorMessage = std::string("Failed to load scene File ") + "data/sceneFile.txt";
			MessageBox(m_MainWindow, errorMessage.c_str(), "Failed to load scene File", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}

	stackValues++;

	// Load all the actors

	lua_pushnil(luaState);
	stackValues++;
	const int tableIndex = -2;
	{
		while (lua_next(luaState, tableIndex))
		{
			//Taking the value pushed by luanext into account
			stackValues++;
			if (lua_type(luaState, -1) != LUA_TTABLE)
			{
				std::cerr << "Value is not a table \n";
				lua_pop(luaState, stackValues);
				return false;
			}
			{

				Engine::SharedPointer<Engine::Actor> newActor(new Engine::Actor());
				if (!LuaHelpers::LoadLuaString(*luaState, "name", stackValues))
				{
					goto OnExit;
				}
				// Store the actors name
				newActor->SetName(lua_tostring(luaState, -1));
				
				lua_pop(luaState, 1);
				stackValues--;

				// Load the controller name
				if (!LuaHelpers::LoadLuaString(*luaState, "controller", stackValues))
				{
					goto OnExit;
				}

				// Store the actors controller
				const char* actorController = lua_tostring(luaState, -1);
				newActor->addController(m_Controllers[actorController]);
				lua_pop(luaState, 1);
				stackValues--;

				// Load the position table
				if (!LuaHelpers::LoadLuaTable(*luaState, "position", stackValues))
				{
					goto OnExit;
				}

				// Store the positions
				Engine::Vector3 position;
				if (!LuaHelpers::LoadFloat3(*luaState, stackValues, position))
				{
					goto OnExit;
				}
				newActor->setPosition(position);
				lua_pop(luaState, 1);
				stackValues--;

				if (strcmp(newActor->GetName(), "flag_1") == 0)//|| strcmp(newActor->GetName(), "flag_2") == 0)
				{
					m_flags[0] = newActor;
					new Engine::TriggerVolume(newActor, Engine::AABB(Engine::Vector3(-1408.0f, 137.0f, -1453.0f), 50.0f), Engine::HashedString("Flag1"), AssignControllerToFlag);
					Engine::World::addActor(newActor);
				}

				if (strcmp(newActor->GetName(), "flag_2") == 0 )//|| strcmp(newActor->GetName(), "flag_2") == 0)
				{
					m_flags[1] = newActor;
					new Engine::TriggerVolume(newActor, Engine::AABB(Engine::Vector3(1408.0f,137.0f,1453.0f), 50.0f), Engine::HashedString("Flag2"), AssignControllerToFlag);
					Engine::World::addActor(newActor);
				}

				// Load the material name
				if (!LuaHelpers::LoadLuaString(*luaState, "material", stackValues))
				{
					goto OnExit;
				}

				// Store the actors material
				std::string actorMaterial = lua_tostring(luaState, -1);
				actorMaterial = "data/" + actorMaterial + ".txt";

				m_Graphics->AssignMaterial(actorMaterial.c_str(), newActor);
				lua_pop(luaState, 1);
				stackValues--;

				// Load the mesh name
				if (!LuaHelpers::LoadLuaString(*luaState, "mesh", stackValues))
				{
					goto OnExit;
				}

				// Store the actors mesh
				std::string actorMesh = lua_tostring(luaState, -1);
				actorMesh = "data/Level/" + actorMesh + ".txt";
				m_Graphics->AssignMesh(actorMesh.c_str(), newActor);
				lua_pop(luaState, 1);
				stackValues--;

				
				// Add actor to world
				//Engine::World::addActor(newActor);
				Engine::World::addRenderable(newActor);
			}

			// To iterate to the next key/value pair, pop the value, but leave the key:
			lua_pop(luaState, 1);
			stackValues--;
		}
	}
	//Popped by lua_next
	stackValues--;

	// Pop the scene table
	lua_pop(luaState, 1);
	
	// Load the asset file into a table at the top of the stack
	{
		const int luaResult = luaL_dofile(luaState, "data/lightingInfo.txt");
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
			std::string errorMessage = std::string("Failed to load lightingInfo File ") + "data/lightingInfo.txt";
			MessageBox(m_MainWindow, errorMessage.c_str(), "Failed to load lightingInfo File", MB_OK | MB_ICONERROR);
			goto OnExit;
		}
	}

	stackValues++;
	// Load the ambient light color
	{
		if (!LuaHelpers::LoadLuaTable(*luaState, "ambientLightColor", stackValues))
		{
			goto OnExit;
		}

		Engine::Vector3 ambientLightColor;
		if (!LuaHelpers::LoadFloat3(*luaState, stackValues, ambientLightColor))
		{
			goto OnExit;
		}
		else
			m_Graphics->SetAmbientLight(ambientLightColor);
		lua_pop(luaState, 1);
		stackValues--;
	}
	// Load the diffuse light color
	{
	if (!LuaHelpers::LoadLuaTable(*luaState, "diffuseLightColor", stackValues))
	{
		goto OnExit;
	}

	Engine::Vector3 diffuseLightColor;
	if (!LuaHelpers::LoadFloat3(*luaState, stackValues, diffuseLightColor))
	{
		goto OnExit;
	}
	else
		m_Graphics->SetDiffuseLight(diffuseLightColor);
	lua_pop(luaState, 1);
	stackValues--;
	}
	// Pop the lighting table
	lua_pop(luaState, 1);
	stackValues--;

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

void Game::SetupPlayer()
{
	
}