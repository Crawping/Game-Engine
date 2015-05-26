#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H
#include "IActorController.h"

class PhysicsController :public Engine::IActorController
{
public:
	void updateActor(Engine::Actor&, float);
};

#endif //PHYSICSCONTROLLER_H