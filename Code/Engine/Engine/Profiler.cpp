#include "Profiler.h"
#include "DebugLog.h"

void Engine::Profiler::addTiming( const char * i_pName, float i_MS )
{
	m_Accumulators[i_pName].m_Count++;
	m_Accumulators[i_pName].m_Sum+=i_MS;
	if(i_MS>m_Accumulators[i_pName].m_Max)
		m_Accumulators[i_pName].m_Max=i_MS;
	if(i_MS<m_Accumulators[i_pName].m_Min)
		m_Accumulators[i_pName].m_Min=i_MS;

}

void Engine::Profiler::PrintAccumulators( void )
{
	for(std::map<std::string, struct Accumulator >::iterator it=m_Accumulators.begin(); it!=m_Accumulators.end(); ++it)
	{
		float Average = it->second.m_Count ? ((float) it->second.m_Sum ) / it->second.m_Count : 0.0f;
		DebugOutput("Profiler: %s \nStatistics \nAverage: %f \nFrames: %i \nTotalTime(Ms): %f \nMaxTime: %f \nMinTime: %f \n",it->first.c_str(), Average, it->second.m_Count,it->second.m_Sum,it->second.m_Max,it->second.m_Min);
	}
}
