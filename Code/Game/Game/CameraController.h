#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include "IActorController.h"
#include "SharedPointer.h"
class CameraController :public Engine::IActorController
{
	Engine::SharedPointer<Engine::Actor> m_follower;
	float m_displacement;
public:
	CameraController(Engine::SharedPointer<Engine::Actor> i_follower) :m_follower(i_follower), m_displacement(400.0){};
	void updateActor(Engine::Actor&, float);
	void setDispacement(float i_displacement)
	{
		if (i_displacement > 50.0f)
			m_displacement = i_displacement;
		else
			i_displacement = 50.0f;

		if (i_displacement > 400.0f)
			m_displacement = 400.0f;
	}


	float getDisplacement()
	{
		return m_displacement;
	}
};

#endif //CAMERACONTROLLER_H