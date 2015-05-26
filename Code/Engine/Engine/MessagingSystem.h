#ifndef MESSAGINGSYSTEM_H
#define MESSAGINGSYSTEM_H
#include <map>
#include <vector>
#include "HashedString.h"
#include "IMessageHandler.h"

namespace Engine
{
	class MessagingSystem
	{
		std::map< HashedString, std::vector< IMessageHandler * > >	m_MessageHandlers;
	public:
		MessagingSystem( ){};
		inline void RegisterMessageHandler( const HashedString & i_Message, IMessageHandler & i_MessageHandler );				//Registers a message and the handlers which need to be called when it is sent
		bool UnregisterMessageHandler( const HashedString & i_Message, IMessageHandler & i_MessageHandler );					//Removes a registered message
		bool SendMessage( const HashedString & i_Message, SharedPointer<Actor> & i_MessageObject );								//Calls the handlers associated with a particular message
	};
}
#include "MessagingSystem.inl"
#endif //MessagingSystem