#ifndef TRIGGERVOLUME_H
#define TRIGGERVOLUME_H
#include "AABB.h"
#include "SharedPointer.h"
#include "Actor.h"
#include "HashedString.h"
#include <vector>
namespace Engine
{
	class TriggerVolume
	{
		static std::vector<TriggerVolume*> m_triggerVolumes;
		AABB m_triggerBox;
		HashedString m_tag;
		SharedPointer<Actor> m_triggerActor;
		void(*m_callbackFunc)(SharedPointer<Actor>,TriggerVolume*);
		bool m_active;
	public:
		TriggerVolume(SharedPointer<Actor> i_actor, AABB i_volume, HashedString i_tag, void(*i_func)(SharedPointer<Actor>, TriggerVolume*))
		{
			m_triggerBox = i_volume;
			m_triggerActor = i_actor;
			m_tag = i_tag;
			m_triggerVolumes.push_back(this);
			m_callbackFunc = i_func;
			m_active = true;
		}

		void SetActive(bool i_value) { m_active = i_value; }
		static void CheckIfIntersecting(Vector3 i_position, HashedString i_tag);
	};
}
#endif // TRIGGERVOLUME_H