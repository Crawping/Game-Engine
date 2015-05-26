#include "precompiled.h"
#include "CameraController.h"
#include "UserInput.h"
#include "Actor.h"


void CameraController::updateActor(Engine::Actor& i_actor, float i_time)
{
	i_actor.setPosition(m_follower->getPosition() - (m_follower->getForward()*(m_displacement-10.0f)));
	i_actor.setRotation(m_follower->getRotation());
}	