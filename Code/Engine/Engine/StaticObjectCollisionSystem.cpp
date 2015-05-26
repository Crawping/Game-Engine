#include "StaticObjectCollisionSystem.h"
#include "Vector3.h"
#include <string>
Engine::StaticObjectCollisionSystem* Engine::StaticObjectCollisionSystem::GetInstance()
{
	static StaticObjectCollisionSystem SingleInstance;
	return &SingleInstance;
}

bool Engine::StaticObjectCollisionSystem::LoadStaticCollisionMesh()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{			
				if (!m_collisionMesh[i][j][k].LoadMeshDataFromFile(std::string(std::string("data/StaticCollisionData/OctreeLeaf[") + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(3-k) + "].txt").c_str(), m_mainWindow))
					return false;
			}
		}
	}
	return true;
}

bool Engine::StaticObjectCollisionSystem::Initialize(HWND i_window)
{
	m_mainWindow = i_window;
	if (!m_collisionTree.InstantiateRoot(2, i_window))
	{
		return false;
	}
	else
		return true;
}

Engine::StaticCollisionInfo Engine::StaticObjectCollisionSystem::CheckForCollision(Engine::Vector3& i_position, Engine::Vector3 i_offset)
{
	
	StaticCollisionInfo currentFrameCollisionStatus;
	currentFrameCollisionStatus.height = -1000000.0f;
	currentFrameCollisionStatus.camDistance = 1000000.0f;

	currentFrameCollisionStatus.backwardXdistance = 1000000.0f;
	currentFrameCollisionStatus.backwardZdistance = 1000000.0f;


	m_collisionTree.CheckForCollision(i_position, i_position+i_offset, currentFrameCollisionStatus, m_collisionTree.GetRoot());

	if (currentFrameCollisionStatus.m_groundCheck)
	{
		i_position.sety(currentFrameCollisionStatus.height + 0.9*100.0f);
	}
	return currentFrameCollisionStatus;
}

void Engine::StaticObjectCollisionSystem::GetCollidedAABBs(Vector3 i_position, Vector3 i_offset, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors)
{
	StaticCollisionInfo currentFrameCollisionStatus;
	m_collisionTree.GetCollidedAABB(i_position, i_offset, 3,currentFrameCollisionStatus, m_collisionTree.GetRoot(), i_collidedAABBs, i_colors);
}
