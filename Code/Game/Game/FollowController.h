#ifndef FOLLOWCONTROLLER_H
#define FOLLOWCONTROLLER_H
#include "IActorController.h"
#include "Vector3.h"
#include "SharedPointer.h"
class FollowController :public Engine::IActorController
{
	Engine::SharedPointer<Engine::Actor> m_follower;
	Engine::Vector3 m_offset;
public:
	FollowController(Engine::SharedPointer<Engine::Actor> i_follower) :m_follower(i_follower){};
	void updateActor(Engine::Actor& i_actor, float i_deltaTime)
	{
		i_actor.setPosition(m_follower->getPosition()-m_offset);
	}

	void SetOffset(Engine::Vector3 i_offset)
	{
		m_offset = i_offset;
	}
};

#endif //FOLLOWCONTROLLER_H