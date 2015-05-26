// AABB struct inline function definitions
//Get Functions
inline float Engine::AABB::getHalfWidth()
{
	return m_HalfWidth;
}
inline float Engine::AABB::getHalfHeight()
{
	return m_HalfHeight;
}
inline float Engine::AABB::getHalfDepth()
{
	return m_HalfDepth;
}

inline Engine::Vector3 Engine::AABB::getWidths()
{
	return m_Widths;
}
inline Engine::Vector3 Engine::AABB::getCenter()
{
	return m_Center;
}

//Set Functions 
Engine::AABB::AABB(Engine::Vector3 i_center, float i_dimensions)
{
	m_HalfWidth = i_dimensions;
	m_HalfHeight = i_dimensions;
	m_HalfDepth = i_dimensions;
	m_Center = i_center;
	m_Widths = Engine::Vector3(m_HalfWidth, m_HalfWidth, m_HalfWidth);
}



