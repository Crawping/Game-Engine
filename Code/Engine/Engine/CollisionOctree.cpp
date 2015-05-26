#include "CollisionOctree.h"
#include "Vector3.h"

void Engine::CollisionOctree::CheckForCollision(Engine::Vector3& i_position, Engine::Vector3& i_offset, Engine::StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node)
{
	if (i_node->CheckForCollision(i_position, i_offset, i_frameCollisionInfo) != 0)
	for (int i = 0; i < 8; i++)
	{
		CheckForCollision(i_position, i_offset, i_frameCollisionInfo, i_node->GetChildByIndex(i));
	}
}

void Engine::CollisionOctree::GetCollidedAABB(Vector3 i_position, Vector3 i_offset, int depth, StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors)
{
	if (depth == 0) return;
	if (i_node->CheckForCollision(i_position, i_offset, i_frameCollisionInfo) != 0)
	{
		i_collidedAABBs->push_back(i_node->GetAABB());
		i_colors->push_back(D3DCOLOR_ARGB(255, 0, 255, 0));

		for (int i = 0; i < 8; i++)
		{
			GetCollidedAABB(i_position, i_offset, depth-1,i_frameCollisionInfo, i_node->GetChildByIndex(i), i_collidedAABBs,i_colors);
		}
	}
	else
	{
		i_collidedAABBs->push_back(i_node->GetAABB());
		i_colors->push_back(D3DCOLOR_ARGB(255, 255, 0, 0));

		for (int i = 0; i < 8; i++)
		{
			GetAABB(i_position, i_offset,depth-1, i_frameCollisionInfo, i_node->GetChildByIndex(i), i_collidedAABBs,i_colors);
		}
	}

}

void Engine::CollisionOctree::GetAABB(Vector3 i_position, Vector3 i_offset,int depth, StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors)
{
	if (depth == 0) return;

	i_collidedAABBs->push_back(i_node->GetAABB());
	i_colors->push_back(D3DCOLOR_ARGB(255, 255, 0, 0));
	for (int i = 0; i < 8; i++)
	{
		GetAABB(i_position, i_offset, depth-1,i_frameCollisionInfo, i_node->GetChildByIndex(i), i_collidedAABBs, i_colors);
	}
}

Engine::OctreeNode* Engine::CollisionOctree::GetRoot()
{
	return &m_root;
}

bool Engine::CollisionOctree::InstantiateRoot(int i_depth, HWND i_window)
{
	float startLoc = -2000.0f;
	float endLoc = 2000.0f;
	float sectors = 2;
	float increment = (endLoc - startLoc) / sectors;
	m_root.SetCoordinate(Vector3(0.0f, 0.0f, 0.0f));
	m_root.SetAABB(AABB(Vector3(0.0f, 0.0f, 0.0f), 2000.0f));
	return InitializeNodes(i_depth-1, &m_root, i_window);
}


bool Engine::CollisionOctree::InitializeNodes(int i_depth, OctreeNode* i_node, HWND i_window)
{
	AABB nodeBox = i_node->GetAABB();
	float startLocx = nodeBox.getCenter().getx() - nodeBox.getHalfWidth();
	float endLocx = nodeBox.getCenter().getx() + nodeBox.getHalfWidth();
	float startLocy = nodeBox.getCenter().gety() - nodeBox.getHalfWidth();
	float endLocy = nodeBox.getCenter().gety() + nodeBox.getHalfWidth();
	float startLocz = nodeBox.getCenter().getz() - nodeBox.getHalfWidth();
	float endLocz = nodeBox.getCenter().getz() + nodeBox.getHalfWidth();
	float sectors = 2;
	float increment = (endLocx - startLocx) / sectors;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				if (i_depth != 0)
				{
					OctreeNode* nodeInstance = new OctreeNode(AABB(Vector3(startLocx + x*increment + increment / 2, startLocy + y*increment + increment / 2, startLocz + z*increment + increment / 2), increment / 2));
					nodeInstance->SetCoordinate(i_node->GetCoordinate()*2 + Vector3(x, y, z));
					if (!InitializeNodes(i_depth - 1, nodeInstance, i_window))
						return false;
					i_node->AddChild(nodeInstance);
				}
				else
				{
					OctreeLeaf* nodeInstance = new OctreeLeaf(AABB(Vector3(startLocx + x*increment + increment / 2, startLocy + y*increment + increment / 2, startLocz + z*increment + increment / 2), increment / 2));
					nodeInstance->SetCoordinate(i_node->GetCoordinate()*2 + Vector3(x, y, z));
					if (!nodeInstance->LoadMesh(std::string(std::string("data/StaticCollisionData/OctreeLeaf[") + std::to_string((int)nodeInstance->GetCoordinate().getx()) + "," + std::to_string((int)nodeInstance->GetCoordinate().gety()) + "," + std::to_string(3-(int)nodeInstance->GetCoordinate().getz()) + "].txt").c_str(), i_window))
						return false;
					i_node->AddChild(nodeInstance);
				}
			}
		}
	}
	return true;
}