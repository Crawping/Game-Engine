#include "precompiled.h"
#include "PhysicsController.h"
#include "UserInput.h"
#include "Actor.h"


void PhysicsController::updateActor(Engine::Actor& i_actor, float i_time)
{
	
	i_actor.setVelocityx(i_actor.getVelocityx() + i_actor.getAccelerationx()*i_time);
	if (i_actor.getVelocityx() > i_actor.getMaxVelocity())
		i_actor.setVelocityx(i_actor.getMaxVelocity());
	if (i_actor.getVelocityx() < -i_actor.getMaxVelocity())
		i_actor.setVelocityx(-i_actor.getMaxVelocity());

	i_actor.setVelocityy(i_actor.getVelocityy() + i_actor.getAccelerationy()*i_time);
	if (i_actor.getVelocityy() > i_actor.getMaxVelocity())
		i_actor.setVelocityy(i_actor.getMaxVelocity());
	if (i_actor.getVelocityy() < -i_actor.getMaxVelocity())
		i_actor.setVelocityy(-i_actor.getMaxVelocity());

	i_actor.setVelocityz(i_actor.getVelocityz() + i_actor.getAccelerationz()*i_time);
	if (i_actor.getVelocityz() > i_actor.getMaxVelocity())
		i_actor.setVelocityz(i_actor.getMaxVelocity());
	if (i_actor.getVelocityz() < -i_actor.getMaxVelocity())
		i_actor.setVelocityz(-i_actor.getMaxVelocity());

	i_actor.setVelocity(i_actor.getVelocity()*0.9f);
	i_actor.setPositionx(i_actor.getPositionx() + i_actor.getVelocityx()*i_time + i_actor.getAccelerationx()*i_time*i_time / 2.0f);
	i_actor.setPositiony(i_actor.getPositiony() + i_actor.getVelocityy()*i_time + i_actor.getAccelerationy()*i_time*i_time / 2.0f);
	i_actor.setPositionz(i_actor.getPositionz() + i_actor.getVelocityz()*i_time + i_actor.getAccelerationz()*i_time*i_time / 2.0f);
}