#include "precompiled.h"
#include "UserController.h"
#include "UserInput.h"
#include "Actor.h"
#include "Networking.h"

void UserController:: updateActor(Engine::Actor& i_actor, float i_time)
{
	if (Engine::Networking::GetInstance()->isInit())
	{
		if (Engine::Networking::GetInstance()->isServerr())
		{
			if (Engine::UserInput::IsKeyPressed('Z'))
			{
				i_actor.setVelocityy(3000.0f);
			}
			if (Engine::UserInput::IsKeyPressed('X'))
			{
				i_actor.setVelocityy(-3000.0f);
			}

			if (Engine::UserInput::IsKeyPressed('W'))
			{
				i_actor.setVelocity(i_actor.getForward()* 3000.0f);

			}

			if (Engine::UserInput::IsKeyPressed('S'))
			{
				i_actor.setVelocity(i_actor.getForward()* -3000.0f);

			}
			if (Engine::UserInput::IsKeyPressed('Q'))
			{
				i_actor.setRotationx(i_actor.getRotationx() - 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('E'))
			{
				i_actor.setRotationx(i_actor.getRotationx() + 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('D'))
			{
				i_actor.setRotationy(i_actor.getRotationy() + 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('A'))
			{
				i_actor.setRotationy(i_actor.getRotationy() - 0.1f);
			}
		}
		else
		{
			if (Engine::UserInput::IsKeyPressed('N'))
			{
				i_actor.setVelocityy(3000.0f);
			}
			if (Engine::UserInput::IsKeyPressed('M'))
			{
				i_actor.setVelocityy(-3000.0f);
			}

			if (Engine::UserInput::IsKeyPressed('I'))
			{
				i_actor.setVelocity(i_actor.getForward()* 3000.0f);

			}

			if (Engine::UserInput::IsKeyPressed('K'))
			{
				i_actor.setVelocity(i_actor.getForward()* -3000.0f);

			}
			if (Engine::UserInput::IsKeyPressed('U'))
			{
				i_actor.setRotationx(i_actor.getRotationx() - 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('O'))
			{
				i_actor.setRotationx(i_actor.getRotationx() + 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('L'))
			{
				i_actor.setRotationy(i_actor.getRotationy() + 0.1f);
			}
			if (Engine::UserInput::IsKeyPressed('J'))
			{
				i_actor.setRotationy(i_actor.getRotationy() - 0.1f);
			}
		}
	}
}