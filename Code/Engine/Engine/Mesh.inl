
inline void Engine::Mesh::SetPosition(const Engine::Vector3 i_Position)
{
	m_Position = i_Position;
}

inline Engine::Vector3 Engine::Mesh::GetPosition() const
{
	return m_Position;
}

inline void Engine::Mesh::SetX(float i_X)
{
	m_Position.setx(i_X);
}


inline void Engine::Mesh::SetY(float i_Y)
{
	m_Position.sety(i_Y);
}

inline void Engine::Mesh::SetZ(float i_Z)
{
	m_Position.setz(i_Z);
}

inline float Engine::Mesh::GetX()
{
	return m_Position.getx();
}

inline float Engine::Mesh::GetY()
{
	return m_Position.gety();
}


inline float Engine::Mesh::GetZ()
{
	return m_Position.getz();
}

