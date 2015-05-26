#include "Timer.h"
#include <Windows.h>
#include "HashedString.h"
#include "SharedPointer.h"
#include "Actor.h"
#include "DebugLog.h"
#include "MessagingSystem.h"


Engine::Timer::Timer()
{
	paused=false;
	QueryPerformanceFrequency(&CounterFrequency);
	CurrentTime.QuadPart = 0;
	PreviousTime.QuadPart = 0;
}

Engine::Timer::Timer(Engine::MessagingSystem& i_MessagingSystem)
{
	paused=false;
	QueryPerformanceFrequency(&CounterFrequency);
	CurrentTime.QuadPart = 0;
	PreviousTime.QuadPart = 0;
	i_MessagingSystem.RegisterMessageHandler("GamePaused",*this);
	i_MessagingSystem.RegisterMessageHandler("GameUnpaused",*this);
}

void Engine::Timer::UpdateTime()
{
	PreviousTime.QuadPart=CurrentTime.QuadPart;
	QueryPerformanceCounter(&CurrentTime);
}

bool Engine::Timer::ProcessMessage( const Engine::HashedString & i_Message, Engine::SharedPointer<Engine::Actor> & i_MessageObject ) 
{
	if(i_Message=="GamePaused")
	{
		paused=true;
		return true;
	}
	if(i_Message=="GameUnpaused")
	{
		paused=false;
		return true;
	}
	{
#ifdef _DEBUG
		DebugOutput("Unknown Message Received \n");
#endif //DebugMessage
		return false;
	}
}