#include "Actor.h"
#include "World.h"
#include "SharedPointer.h"
inline void Engine::World::addActor(Engine::SharedPointer<Actor> i_Actor)
{
	m_Actors.push_back(i_Actor);
}

inline void Engine::World::addRenderable(Engine::SharedPointer<Actor> i_Actor)
{
	m_Renderables.push_back(i_Actor);
}

inline std::vector <Engine::SharedPointer<Engine::Actor>>& Engine::World::getActorList()
{
	return m_Actors;
}

inline std::vector <Engine::SharedPointer<Engine::Actor>>& Engine::World::getRenderableList()
{
	return m_Renderables;
}

inline Engine::Timer* Engine::World::getTimer()
{
	return &m_Clock;
}

inline unsigned int Engine::World::getLives()
{
	return NumberOfLives;
}

inline void Engine::World::setLives(unsigned int i_Lives)
{
	NumberOfLives=i_Lives;
}

inline Engine::MessagingSystem& Engine::World::getMessagingSystem()
{
	return m_MessagingSystem;
}

inline Engine::Profiler& Engine::World::getProfiler()
{
	return m_Profiler;
}

inline unsigned int Engine::World::getScore()
{
	return m_Score;
}

inline void Engine::World::setScore(const unsigned int i_Score)
{
	m_Score = i_Score;
}