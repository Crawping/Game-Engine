#include "precompiled.h"
#include"Networking.h"
//#include "DebugFunctionWin32.h"
#include"World.h"

using namespace Engine;


void Networking::initialize(bool iServer, char iIp[])
{
	strcpy_s(ip, 512, iIp);
	m_isServer = iServer;




	peer = RakNet::RakPeerInterface::GetInstance();





	if (!m_isServer)
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);

	}
	else {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		m_isServer = true;
	}

	if (m_isServer)
	{
		//DEBUG_PRINT("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{



		//DEBUG_PRINT("Starting the client.\n");
		peer->Connect(ip, SERVER_PORT, 0, 0);

	}


	isInitialized = true;

}


bool Networking::isInit()
{
	return isInitialized;
}


bool Networking::isServerr()
{
	return m_isServer;
}
void Networking::Update()
{
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			peer->Send(reinterpret_cast<char*>(&selfPlayerData), sizeof(selfPlayerData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
			break;
	

		case ID_GAME_MESSAGE_1:
		{						
			const GamePacket *message = reinterpret_cast<const GamePacket *>(packet->data);
			otherPlayerData = *message;

			peer->Send(reinterpret_cast<char*>(&selfPlayerData), sizeof(selfPlayerData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
			break;

		default:
			break;
		}
	}
}