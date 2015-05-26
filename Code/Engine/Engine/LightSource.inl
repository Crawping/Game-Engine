
inline Engine::Vector3 Engine::LightSource::getDirection() const
{
	return m_direction;
}

inline void Engine::LightSource::setDirection(Engine::Vector3 i_direction)
{
	m_direction = i_direction;
}

inline Engine::Vector3 Engine::LightSource::getColor() const
{
	return m_color;
}

inline void Engine::LightSource::setColor(Vector3 i_color)
{
	m_color = i_color;
}

inline Engine::Vector3 Engine::LightSource::getPosition() const
{
	return m_position;
}

inline void Engine::LightSource::setPosition(Vector3 i_position)
{
	m_position = i_position;

	Vector3 direction = Vector3(0.0f, 0.0f, 0.0f) - m_position;
	m_direction = direction.normalized();
}