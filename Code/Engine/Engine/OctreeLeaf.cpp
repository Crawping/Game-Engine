#include "OctreeLeaf.h"
#include "Vector3.h"
#include<math.h>
// THE CONTENTS OF THE FOLLOWING FUNCTION HAVE BEEN ACQUIRED FROM THE INTERNET
// Given segment pq and triangle abc, returns whether segment intersects
// triangle and if so, also returns the barycentric coordinates (u,v,w)
// of the intersection point
int Engine::OctreeLeaf::IntersectSegmentTriangle(Engine::Vector3 p, Engine::Vector3 q, Engine::Vector3 a, Engine::Vector3 b, Engine::Vector3 c,
	float &u, float &v, float &w, float &t)
{
	Engine::Vector3 ab = b - a;
	Engine::Vector3 ac = c - a;
	Engine::Vector3 qp = p - q;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	Engine::Vector3 n = ab.cross(ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = qp*n;
	if (d <= 0.0f) return 0;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects if 0 <= t. Segment intersects if 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	Engine::Vector3 ap = p - a;
	t = ap*n;
	if (t < 0.0f) return 0;
	if (t > d) return 0; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	Engine::Vector3 e = qp.cross(ap);
	v = ac*e;
	if (v < 0.0f || v > d) return 0;
	w = -(ab*e);
	if (w < 0.0f || v + w > d) return 0;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;
	return 1;
}


int Engine::OctreeLeaf::CheckForCollision(Vector3& i_position, Vector3& i_end, StaticCollisionInfo& currentFrameCollisionStatus)
{
	Vector3 i_offset = i_end - i_position;
	if (m_sectorMeshes.GetVertices() == nullptr)
	{
		return 0;
	}
	float u = 0;
	float v = 0;
	float w = 0;
	float t;
	float height;
	float xDist = 10000.0f;
	float zDist = 10000.0f;
	StaticCollisionMesh* m_collisionMesh = &m_sectorMeshes;
	for (unsigned int i = 0; i < m_collisionMesh->GetNumberOfVertices(); i += 3)
	{

		// Ground check
		if (IntersectSegmentTriangle(i_position, i_position + Vector3(0.0f, -100.0f, 0.0f), m_collisionMesh->GetVertices()[i], m_collisionMesh->GetVertices()[i + 2], m_collisionMesh->GetVertices()[i + 1], u, v, w, t) == 1)
		{			
			currentFrameCollisionStatus.m_groundCheck = true;
			height = u*m_collisionMesh->GetVertices()[i].gety() + v*m_collisionMesh->GetVertices()[i + 2].gety() + w*m_collisionMesh->GetVertices()[i + 1].gety();
			if (height>currentFrameCollisionStatus.height)
			{
				currentFrameCollisionStatus.height = height;
			}
				
		}

		// Forward Check
		if (IntersectSegmentTriangle(i_position, i_position + Vector3(i_offset.getx(), 0.0f, i_offset.getz()), m_collisionMesh->GetVertices()[i], m_collisionMesh->GetVertices()[i + 2], m_collisionMesh->GetVertices()[i + 1], u, v, w, t) == 1)
		{
			
			if (m_collisionMesh->GetVertices()[i].getx() == m_collisionMesh->GetVertices()[i + 1].getx())
				currentFrameCollisionStatus.m_forwardX = true;
			else
				currentFrameCollisionStatus.m_forwardZ = true;
			

			xDist = u*m_collisionMesh->GetVertices()[i].getx() + v*m_collisionMesh->GetVertices()[i + 2].getx() + w*m_collisionMesh->GetVertices()[i + 1].getx();
			xDist = abs(i_position.getx() - xDist);
			zDist = u*m_collisionMesh->GetVertices()[i].getz() + v*m_collisionMesh->GetVertices()[i + 2].getz() + w*m_collisionMesh->GetVertices()[i + 1].getz();
			zDist = abs(i_position.getz() - zDist);

			if (xDist < currentFrameCollisionStatus.forwardXdistance)	currentFrameCollisionStatus.forwardXdistance = xDist;
			if (zDist < currentFrameCollisionStatus.forwardZdistance)	currentFrameCollisionStatus.forwardZdistance = zDist;

			

		}

		// Backward Check

		if (IntersectSegmentTriangle(i_position, i_position - Vector3(i_offset.getx()*4, 0.0f, i_offset.getz()*4), m_collisionMesh->GetVertices()[i], m_collisionMesh->GetVertices()[i + 2], m_collisionMesh->GetVertices()[i + 1], u, v, w, t) == 1)
		{			
			if (m_collisionMesh->GetVertices()[i].getx() == m_collisionMesh->GetVertices()[i + 1].getx())
				currentFrameCollisionStatus.m_backwardX = true;
			else
				currentFrameCollisionStatus.m_backwardZ = true;

			xDist = u*m_collisionMesh->GetVertices()[i].getx() + v*m_collisionMesh->GetVertices()[i + 2].getx() + w*m_collisionMesh->GetVertices()[i + 1].getx();
			xDist = abs(i_position.getx() - xDist);
			zDist = u*m_collisionMesh->GetVertices()[i].getz() + v*m_collisionMesh->GetVertices()[i + 2].getz() + w*m_collisionMesh->GetVertices()[i + 1].getz();
			zDist = abs(i_position.getz() - zDist);

			if (xDist < currentFrameCollisionStatus.backwardXdistance)	currentFrameCollisionStatus.backwardXdistance = xDist;
			if (zDist < currentFrameCollisionStatus.backwardZdistance)	currentFrameCollisionStatus.backwardZdistance = zDist;

		}
		float camDist;
		// Cam Ground check
		if (IntersectSegmentTriangle(i_position, i_position - Vector3(i_offset.getx() * 4, i_offset.gety() * 4, i_offset.getz() * 4), m_collisionMesh->GetVertices()[i], m_collisionMesh->GetVertices()[i + 2], m_collisionMesh->GetVertices()[i + 1], u, v, w, t) == 1)
		{
			xDist = u*m_collisionMesh->GetVertices()[i].getx() + v*m_collisionMesh->GetVertices()[i + 2].getx() + w*m_collisionMesh->GetVertices()[i + 1].getx();
			zDist = u*m_collisionMesh->GetVertices()[i].getz() + v*m_collisionMesh->GetVertices()[i + 2].getz() + w*m_collisionMesh->GetVertices()[i + 1].getz();
			currentFrameCollisionStatus.m_camCollided = true;
			camDist =sqrt((i_position.getx() - xDist)*(i_position.getx() - xDist) + (i_position.getz() - zDist)*(i_position.getz() - zDist));
			if (camDist < currentFrameCollisionStatus.camDistance) currentFrameCollisionStatus.camDistance = camDist;
		}
	}
	return 0;

}
Engine::StaticCollisionMesh Engine::OctreeLeaf::GetMeshes()
{	
	return m_sectorMeshes;
}

bool Engine::OctreeLeaf::LoadMesh(const char* i_path, HWND i_window)
{
	if (!m_sectorMeshes.LoadMeshDataFromFile(i_path, i_window))
		return false;
	else
		return true;
}

