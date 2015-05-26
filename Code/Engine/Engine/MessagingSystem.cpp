#include "MessagingSystem.h"
#include "IMessageHandler.h"
#include "HashedString.h"
#include <assert.h>

bool Engine::MessagingSystem::UnregisterMessageHandler( const Engine::HashedString & i_Message, Engine::IMessageHandler & i_MessageHandler )
{
	if(m_MessageHandlers.find(i_Message)!=m_MessageHandlers.end())
	{
		for(unsigned int i=0;i<m_MessageHandlers[i_Message].size();i++)
		{
			if(m_MessageHandlers[i_Message][i]==&i_MessageHandler)
				m_MessageHandlers[i_Message].erase(m_MessageHandlers[i_Message].begin()+i);
		}
		if(m_MessageHandlers[i_Message].size()==0)
				m_MessageHandlers.erase(i_Message);
		return true;
	}
	else
	{
#ifdef _DEBUG
		DebugOutput("Trying to Unregister Handler to a message which does not exist \n");
#endif //DebugMessage
		return false;
	}
}
bool Engine::MessagingSystem::SendMessage( const Engine::HashedString & i_Message, Engine::SharedPointer<Engine::Actor> & i_MessageObject )
{
	if(m_MessageHandlers.find(i_Message)!=m_MessageHandlers.end())
	{
		for(unsigned int i=0; i<m_MessageHandlers[i_Message].size();i++)
		{
			if(!m_MessageHandlers[i_Message][i]->ProcessMessage(i_Message,i_MessageObject))
			{
#ifdef _DEBUG
				DebugOutput("Message Processing Failed \n");
#endif //DebugMessage
			}
		}
		return true;
	}
	else
	{
#ifdef _DEBUG
		DebugOutput("Invalid Message \n");
#endif //DebugMessage
		return false;
	}
}