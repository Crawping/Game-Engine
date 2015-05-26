#ifndef IACTORCONTROLLER_H
#define IACTORCONTROLLER_H
namespace Engine
{
	class Actor;
	class IActorController
	{

	public:
		virtual void updateActor(Actor&,float)  = 0;
		virtual ~IActorController(){};
	};
}
#endif