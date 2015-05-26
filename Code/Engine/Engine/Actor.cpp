#include "Actor.h"
Engine::MemoryPool* Engine::Actor::m_pActorPool=nullptr;
void Engine::Actor::update(float i_DeltaTime)
{
	for (int i = 0; i < m_Controllers.size();i++)
		m_Controllers[i]->updateActor( *this,i_DeltaTime);
}


void Engine::Actor::randomizePosition()
{
	setPositionx(static_cast <float> (rand()%(10 * 5) - 5 * 5));
	setPositiony(static_cast <float> (rand()%(10 * 5) - 5 * 5));
	setPositionz(static_cast <float> (rand()%(10 * 5) - 5 * 5));
}

void* Engine::Actor::operator new(size_t i_size)
{
	if (m_pActorPool == nullptr)
		m_pActorPool = MemoryPool::createMemoryPool(sizeof(Actor), 5100);
	return m_pActorPool->allocateMemory(sizeof(Actor));
}

void Engine::Actor::operator delete(void* i_ptr)
{
	m_pActorPool->freeMemory(i_ptr, sizeof(Actor));
}

D3DXMATRIX Engine::Actor::GetTransform()
{
	D3DXMATRIX transform;
	{
		D3DXMatrixTransformation(&transform, NULL, NULL, NULL, NULL,
			NULL, &m_Position.getD3DVector3());
	}
	return transform;
}

void Engine::Actor::SetName(const char* i_name)
{
	m_pName = _strdup(i_name);
}