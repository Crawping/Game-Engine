
// OctreeNode Class inline function definitions
inline void Engine::OctreeNode::SetCoordinate(Vector3 i_value)
{
	m_coordinate = i_value;
}

inline Engine::Vector3 Engine::OctreeNode::GetCoordinate()
{
	return m_coordinate;
}

inline void Engine::OctreeNode::SetAABB(AABB i_box)
{
	m_AABB = i_box;
}

inline Engine::AABB Engine::OctreeNode::GetAABB()
{
	return m_AABB;
}
inline void Engine::OctreeNode::AddChild(OctreeNode* i_node)
{
	m_children[m_childCount] = i_node;
	m_childCount++;
}

inline Engine::OctreeNode* Engine::OctreeNode::GetChildByIndex(int i_index)
{
	return m_children[i_index];
}
