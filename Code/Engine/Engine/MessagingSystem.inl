#include "HashedString.h"
#include "IMessageHandler.h"
#include "MessagingSystem.h"
#include <assert.h>

inline void Engine::MessagingSystem::RegisterMessageHandler( const Engine::HashedString & i_Message, Engine::IMessageHandler & i_MessageHandler )
{
	assert(&i_MessageHandler!=NULL);
	m_MessageHandlers[i_Message].push_back(&i_MessageHandler);
}

