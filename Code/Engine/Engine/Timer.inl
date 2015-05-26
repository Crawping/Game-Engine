#include "Timer.h"
#include <Windows.h>
#include "DebugLog.h"
__int64 Engine::Timer::Time()
{
	return CurrentTime.QuadPart;
}

float Engine::Timer::DeltaTime_ms()
{
	if(paused)
		return 0.0f;
	if(PreviousTime.QuadPart==0)
		return 1.0f/60.0f;
	float Delta=static_cast<float>(CurrentTime.QuadPart-PreviousTime.QuadPart)/static_cast<float>(CounterFrequency.QuadPart);
	if(Delta<=1)
	{
		return Delta;
	}
	else
		return 1.0f/60.0f;	
}
