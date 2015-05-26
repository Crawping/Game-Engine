#ifndef IMESSAGEHANDLER_H
#define IMESSAGEHANDLER_H
#include "Actor.h"
#include "HashedString.h"
#include "SharedPointer.h"
namespace Engine
{
	class IMessageHandler
	{
		public:
			virtual ~IMessageHandler(){};
			virtual bool ProcessMessage( const HashedString & i_Message, SharedPointer<Actor> & i_MessageObject ) = 0;
	};
}
#endif //IMessageHandler