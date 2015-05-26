#ifndef PROFILER_H
#define PROFILER_H

#include <assert.h>
#include <map>
#include <string>
#include "Timer.h"
#include "ScopedTimer.h"
namespace Engine
{

	class Profiler
	{
		struct Accumulator
		{
			unsigned int	m_Count;
			float	m_Sum;
			float	m_Max;
			float	m_Min;

			Accumulator()
			{
				m_Count = 0;
				m_Sum = 0;
				m_Max = 0;
				m_Min = FLT_MAX;
			}
		} ;

		std::map<std::string, struct Accumulator> m_Accumulators;

	public:
		void addTiming( const char * i_pName, float i_MS );									//Add the time from the creation to the destruction of the profiler

		void PrintAccumulators( void );														//Print the accumulator information
	} ; 
}


#if defined ( ENABLE_PROFILING)
#define TO_STRING_HELPER(str) #str
#define TO_STRING(str) TO_STRING_HELPER(str)
#define PROFILE_CONCAT(left, right) left##right
#define PROFILE_SCOPE_BEGIN(str)	{ Engine::ScopedTimer PROFILE_CONCAT(str, __LINE__)(#str "Line:" TO_STRING(__LINE__));
#define PROFILE_SCOPE_END()			}

#define PROFILE_UNSCOPED(str)		Engine::ScopedTimer PROFILE_CONCAT(str, __LINE__)( str );
#define PROFILE_PRINT_RESULTS()		Engine::World::getProfiler().PrintAccumulators();

#else
#define PROFILE_SCOPE_BEGIN(str)	__noop();
#define PROFILE_SCOPE_END()			__noop();
#define PROFILE_UNSCOPED(str)		__noop();
#define PROFILE_PRINT_RESULTS()		__noop();
#endif // ENABLE_PROFILING

#endif // __PROFILER_H