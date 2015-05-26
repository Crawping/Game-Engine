
inline Engine::BoundingBox& Engine::CollisionSystem::getBoundingBox()
{
	return m_Collider;
}

inline void Engine::CollisionSystem::setBoundingBox(const Engine::Vector3 i_Offset, const float i_HalfWidth, const float i_HalfHeight,const float i_HalfDepth)
{
	m_Collider.setOffset(i_Offset);
	m_Collider.setHalfWidth(i_HalfWidth);
	m_Collider.setHalfHeight(i_HalfHeight);
	m_Collider.setHalfDepth(i_HalfDepth);
}