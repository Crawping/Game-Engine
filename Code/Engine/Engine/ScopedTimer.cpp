#include "ScopedTimer.h"
#include "World.h"
#include "DebugLog.h"
Engine::ScopedTimer::ScopedTimer( const char * i_pName ) :	m_pScopeName( i_pName )
{
	ScopedTime.UpdateTime();
#if defined(_DEBUG)
	DebugOutput("Scoped Timer %s started at Tick %dld\n", m_pScopeName ? m_pScopeName : "Unnamed", ScopedTime.Time());
#endif

	assert( i_pName );
}

Engine::ScopedTimer::~ScopedTimer()
{
	ScopedTime.UpdateTime();

#if defined(_DEBUG)
	DebugOutput("Scoped Timer %s ended at Tick %lld ran for %d \n", m_pScopeName ? m_pScopeName : "Unnamed", ScopedTime.Time(), ScopedTime.DeltaTime_ms());
#endif

	Engine::World::getProfiler().addTiming( m_pScopeName,ScopedTime.DeltaTime_ms() );
}