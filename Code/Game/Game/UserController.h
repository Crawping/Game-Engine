#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H
#include "IActorController.h"

class UserController :public Engine::IActorController
{
public:
	void updateActor(Engine::Actor&, float);
};

#endif // USERCONTROLLER_H