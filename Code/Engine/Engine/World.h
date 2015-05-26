#ifndef WORLD_H
#define WORLD_H
#include "Actor.h"
#include "SharedPointer.h"
#include "MessagingSystem.h"
#include "Profiler.h"
#include "Vector3.h"
#include <vector>
namespace Engine
{
	class Timer;
	class World
	{
		static std::vector <SharedPointer<Actor>> m_Actors;
		static std::vector <SharedPointer<Actor>> m_Renderables;

		static Profiler m_Profiler;
		static MessagingSystem m_MessagingSystem;
		static Timer m_Clock;
		static unsigned int NumberOfLives;
		static unsigned int m_Score;
	public: 
		//Get Functions
		static inline std::vector <SharedPointer<Actor>>& getActorList();
		static inline std::vector <SharedPointer<Actor>>& getRenderableList();
		static inline Timer* getTimer();
		static inline unsigned int getLives();
		static SharedPointer<Actor> getClosestActor(const SharedPointer<Actor>);	
		static inline MessagingSystem& getMessagingSystem();
		static inline Profiler& getProfiler();
		static inline unsigned int getScore();

		//Set Functions
		static inline void setLives(unsigned int);
		static inline void setScore(const unsigned int);
		static void deleteActor(unsigned int);
		static void deleteActors();
		
		static inline void addActor(SharedPointer<Actor>);
		static inline void addRenderable(SharedPointer<Actor>);

		static SharedPointer<Actor> createActor();
		static SharedPointer<Actor> createActor(const char*,const char*, const Vector3, const Vector3, const Vector3, IActorController*, const float);

		static void checkForDeath();
	};
}
#include "World.inl"
#endif