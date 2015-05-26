#include "World.h"
#include "Actor.h"
#include "Timer.h"
#include "SharedPointer.h"
#include <assert.h>

std::vector<Engine::SharedPointer<Engine::Actor>> Engine::World::m_Actors;
std::vector<Engine::SharedPointer<Engine::Actor>> Engine::World::m_Renderables;


Engine::MessagingSystem Engine::World::m_MessagingSystem;
Engine::Timer Engine::World::m_Clock(m_MessagingSystem);
Engine::Profiler Engine::World::m_Profiler;
unsigned int Engine::World::NumberOfLives=3;
unsigned int Engine::World::m_Score = 0;
void Engine::World::deleteActor(unsigned int i_Index)
{
	Engine::SharedPointer<Actor> temp=m_Actors[i_Index];
	m_Actors[i_Index]=m_Actors[m_Actors.size()-1];
	m_Actors[m_Actors.size()-1]=temp;
	m_Actors.pop_back();
}

void Engine::World::deleteActors()
{
	while(m_Actors.size()!=0)
	{
		m_Actors.pop_back();
	}
	std::vector<Engine::SharedPointer<Engine::Actor>>().swap(m_Actors);

	while (m_Renderables.size() != 0)
	{
		m_Renderables.pop_back();
	}
	std::vector<Engine::SharedPointer<Engine::Actor>>().swap(m_Renderables);
}

Engine::SharedPointer<Engine::Actor> Engine::World::getClosestActor(const Engine::SharedPointer<Actor> i_pActor)
{
	Engine::SharedPointer<Actor> ClosestActor;
	float Distance=600.0f;
	float temp=0;
	for(unsigned int i=0;i<m_Actors.size();i++)
	{
		if(m_Actors[i]!=i_pActor)
		{
			temp= sqrt( (i_pActor.operator->()->getPositionx() - m_Actors[i]->getPositionx())*(i_pActor->getPositionx() - m_Actors[i]->getPositionx()) + 
				(i_pActor->getPositiony() - m_Actors[i]->getPositiony())*(i_pActor->getPositiony() - m_Actors[i]->getPositiony()) +
				(i_pActor->getPositionz() - m_Actors[i]->getPositionz())*(i_pActor->getPositionz() - m_Actors[i]->getPositionz()));
			if(temp<=Distance)
			{
				ClosestActor=m_Actors[i];
				Distance=temp;
			}
		}
	}
	assert(ClosestActor!=NULL);
	return ClosestActor;
}

Engine::SharedPointer<Engine::Actor> Engine::World::createActor()
{

	Actor* pActor= new Actor();
	m_Actors.push_back(SharedPointer<Actor>(pActor));
	return m_Actors[m_Actors.size()-1];   //Return by object, not pointer

}


Engine::SharedPointer<Engine::Actor> Engine::World::createActor(const char* i_pName, const char* i_pType, const Vector3 i_Position, const Vector3 i_Velocity, const Vector3 i_Acceleration, 
																IActorController* i_Controller, const float i_MaxVelocity)
{
	Actor* pActor= new Actor(i_pName, i_pType, i_Position, i_Velocity, i_Acceleration,i_Controller, i_MaxVelocity);
	m_Actors.push_back(SharedPointer<Actor>(pActor));
	return m_Actors[m_Actors.size()-1];   //Return by object, not pointer
}


void Engine::World::checkForDeath()
{
	unsigned int i=0;
	while(i<m_Actors.size())
	{
		if(m_Actors[i]->getMarkedForDeath())
		{
			deleteActor(i);
		}
		else
		{
			i++;
		}
	}
}

