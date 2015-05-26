#include "TriggerVolume.h"
std::vector<Engine::TriggerVolume*> Engine::TriggerVolume::m_triggerVolumes;

void Engine::TriggerVolume::CheckIfIntersecting(Engine::Vector3 i_position, HashedString i_tag)
{
	for (int i = 0; i < m_triggerVolumes.size(); i++)
	{
		if (!m_triggerVolumes[i]->m_active) continue;
		if (m_triggerVolumes[i]->m_tag.Get() != i_tag.Get()) continue;
		if (abs(i_position.getx() - m_triggerVolumes[i]->m_triggerBox.getCenter().getx())>m_triggerVolumes[i]->m_triggerBox.getHalfWidth()) continue;
		if (abs(i_position.gety() - m_triggerVolumes[i]->m_triggerBox.getCenter().gety())>m_triggerVolumes[i]->m_triggerBox.getHalfHeight()) continue;
		if (abs(i_position.getz() - m_triggerVolumes[i]->m_triggerBox.getCenter().getz())>m_triggerVolumes[i]->m_triggerBox.getHalfDepth()) continue;
		m_triggerVolumes[i]->m_callbackFunc(m_triggerVolumes[i]->m_triggerActor,m_triggerVolumes[i]);
	}
}