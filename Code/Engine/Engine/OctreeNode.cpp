#include "OctreeNode.h"

//Checks to see if the ray intersects the AABB for the particular octree node
int Engine::OctreeNode::CheckForCollision(Vector3& p0, Vector3& p1, StaticCollisionInfo& i_frameCollisionInfo)
{
	Vector3 c = m_AABB.getCenter(); // Box center-point
	Vector3 e = m_AABB.getWidths(); // Box halflength extents
	Vector3 m = (p0 + p1) * 0.5f; // Segment midpoint
	Vector3 d = p1 - m; // Segment halflength vector
	m = m - c; // Translate box and segment to origin

	// Try world coordinate axes as separating axes
	float adx = abs(d.getx());
	if (abs(m.getx()) > e.getx() + adx) return 0;
	float ady = abs(d.gety());
	if (abs(m.gety()) > e.gety() + ady) return 0;
	float adz = abs(d.getz());
	if (abs(m.getz()) > e.getz() + adz) return 0;

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis
	adx += EPSILON;
	ady += EPSILON;
	adz += EPSILON;

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.gety() * d.getz() - m.getz() * d.gety()) > e.gety() * adz + e.getz() * ady) return 0;
	if (abs(m.getz() * d.getx() - m.getx() * d.getz()) > e.getx() * adz + e.getz() * adx) return 0;
	if (abs(m.getx() * d.gety() - m.gety() * d.getx()) > e.getx() * ady + e.gety() * adx) return 0;

	// No separating axis found; segment must be overlapping AABB
	return 1;
}

