#ifndef NULLCONTROLLER_H
#define NULLCONTROLLER_H
#include "IActorController.h"

class NullController :public Engine::IActorController
{
public:
	void updateActor(Engine::Actor&, float){};
};

#endif // NULLCONTROLLER_H