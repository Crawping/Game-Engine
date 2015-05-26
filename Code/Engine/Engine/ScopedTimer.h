#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H

#include "Timer.h"
namespace Engine
{
	class ScopedTimer
	{
		const char *		m_pScopeName;
		Timer		ScopedTime;
	public:
		ScopedTimer( const char *);
		~ScopedTimer();
	};
}

#endif //SCOPEDTIMER_H
