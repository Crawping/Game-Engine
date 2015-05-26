#ifndef TIMER_H
#define TIMER_H 
#include <Windows.h>
#include "IMessageHandler.h"
#include "HashedString.h"
#include "SharedPointer.h"
#include "MessagingSystem.h"
#include "Actor.h"
namespace Engine
{
	class Timer:public IMessageHandler
	{
		LARGE_INTEGER CurrentTime;
		LARGE_INTEGER PreviousTime;
		LARGE_INTEGER CounterFrequency;
		bool paused;
	public:
		Timer();											//Default Constructor
		Timer(MessagingSystem&);							//Constructs Timer linked to Messaging System
		void UpdateTime();									//Loads the value of the HighRes counter into CurrentTime and moves the previous value into PreviousTimer
		inline __int64 Time();								//Returns the value of CurrentTimer
		inline float DeltaTime_ms();							//Returns the value of the difference between CurrentTimer and PreviousTimer in Milliseconds
		bool ProcessMessage( const HashedString& , SharedPointer<Actor>& );
		~Timer(){};
	} ;
}
#include "Timer.inl"
#endif
