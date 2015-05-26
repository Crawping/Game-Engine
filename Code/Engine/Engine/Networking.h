#ifndef NETWORKING_H
#define NETWORKING_H

// Header Files
//=============

#include "precompiled.h"

#include "../Source/RakPeerInterface.h"
#include "../Source/MessageIdentifiers.h"
#include "../Source/BitStream.h"
#include "../Source/RakNetTypes.h"  // MessageID
#include "Vector3.h"



#define MAX_CLIENTS 10
#define SERVER_PORT 60000



enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};
namespace Engine
{

	struct GamePacket
	{
		unsigned char typeId; // type here
		Vector3 position;
		Vector3 rotation;
		Vector3 flagPos;
		int score;
		bool flagPicked;
		bool restart;
		bool shiftPressed;
		GamePacket()
		{
			score = 0;
			flagPicked = false;
			shiftPressed = false;
			typeId = static_cast<unsigned char>((RakNet::MessageID)ID_GAME_MESSAGE_1);
		}

	};

	class Networking
	{
		RakNet::RakPeerInterface *peer;
		bool m_isServer;
		RakNet::Packet *packet;
		char ip[512];
		bool isInitialized;
		bool otherPlayerSpawed;
		Networking()
		{
			otherPlayerSpawed = false;
			isInitialized = false;
			otherPlayerData.position = Vector3(10000.0f, 0.0f, 0.0f);
		}
	public:

		static Networking* GetInstance()
		{
			static Networking* objectInstance;
			if (objectInstance == nullptr)
				objectInstance = new Networking;
			return objectInstance;
			
		}
		GamePacket otherPlayerData;
		GamePacket selfPlayerData;
		void initialize(bool iServer, char iIp[] = "127.0.0.1");
		void Update();
		bool isInit();
		bool isServerr();
		
		~Networking()
		{
			RakNet::RakPeerInterface::DestroyInstance(peer);
		}
	};
}


#endif //NETWORKING_H