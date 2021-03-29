#include "Server.h"

Server::Server(int port)
{
	WSADATA wsa_data;
	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	assert(result == 0);		//DLL not found

	hints =
	{
		AI_PASSIVE,
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	};

	//Resolve the local address and port to be used by the server
	result = getaddrinfo(nullptr, "8888", &hints, &info);
	if (result != 0)
	{
		printf("getaddrinfo() failed: %d\n", result);
		WSACleanup();
		exit(0);
	}

	printf("\nSuccessfully opened the port...");

	//Create server socket
	listen_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		printf("Unable to create server socket: %d\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		exit(0);
	}

	printf("\nThe server socket was succesfully created...");

	//Setup the TCP listening socket
	result = bind(listen_socket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("bind() failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(listen_socket);
		WSACleanup();
		exit(0);
	}
	else
		freeaddrinfo(info);

	printf("\nThe listening socket was successfully setup...");

	//Listen for a single connection
	result = listen(listen_socket, 1);
	if (result == SOCKET_ERROR)
	{
		printf("listen() failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		exit(0);
	}

	serverPtr = this;
}

Server::~Server()
{
	//Delete things here
	serverPtr = nullptr;
	delete serverPtr;
} 

void Server::ListenForNewConnections()
{
	//Accept a client socket
	SOCKET client_socket = accept(listen_socket, nullptr, nullptr);
	if (client_socket == INVALID_SOCKET)
	{
		printf("accept() failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		exit(0);
	}
	else
	{
		printf("\nA client has been connected...");
		mClientConnections.push_back(client_socket);
		mClientAvailable.push_back(true);
		mThreadActive.push_back(true);
		//Tell new client if there are 2 (or more) clients on the server, allowing matchmaking
		GetUsername(mConnections);

		mConnectionThreads.push_back(std::thread(ClientHandler, mConnections));
		mConnections++;
	}

}

void Server::ClientHandler(int index)
{
	//Packet
	PACKET mPacketType;

	while (true)
	{
		//Receive Messages
		if (!serverPtr->GetPacketType(index, mPacketType))
			break;

		if (!serverPtr->ProcessPacket(index, mPacketType))
			break;

		if (!serverPtr->mThreadActive[index])
		{
			printf("\nUser %d disconnected due to other client disconnection", index);		//In game, send bool to check if still connected, disconnect if not
			break;
		}
	}

	printf("\nLost Connection with User: %d", index);
	serverPtr->CloseConnection(index);
	serverPtr->mConnectionThreads[index].detach();
	serverPtr->mThreadActive[index] = false;
	//serverPtr->mConnections--;
	//serverPtr->usernames.erase(serverPtr->usernames.begin() + index);
	//serverPtr->mClientConnections.erase(serverPtr->mClientConnections.begin() + index);
	//serverPtr->mConnectionThreads[index].detach();
	//serverPtr->mConnectionThreads.erase(serverPtr->mConnectionThreads.begin() + index);
	//Delete from vector of booleans
	//serverPtr->mClientAvailable.erase(serverPtr->mClientAvailable.begin() + index);
	//Remove pair of int vectors, turn one that didnt connect to available
	//for (size_t i = 0; i < serverPtr->mMatchups.size(); i++)
	//{
	//	if (serverPtr->mMatchups[i].first == index)
	//	{
	//		printf("\nMatchup between %d and %d has been cancelled...", serverPtr->mMatchups[i].first, serverPtr->mMatchups[i].second);
	//		serverPtr->mClientAvailable[serverPtr->mMatchups[i].second] = false;		//Makes other user available again
	//		serverPtr->mMatchups.erase(serverPtr->mMatchups.begin() + i);			//Erases matchup
	//	}
	//	else if (serverPtr->mMatchups[i].second == index)
	//	{
	//		printf("\nMatchup between %d and %d has been cancelled...", serverPtr->mMatchups[i].first, serverPtr->mMatchups[i].second);
	//		serverPtr->mClientAvailable[serverPtr->mMatchups[i].first] = false;		//Makes other user available again
	//		serverPtr->mMatchups.erase(serverPtr->mMatchups.begin() + i);			//Erases matchup
	//	}
	//}
	//Debug out messages
	printf("\nLifetime Connections on server: %d ", serverPtr->mConnections);
}

void Server::CloseConnection(int index)
{
	if (closesocket(mClientConnections[index]) == SOCKET_ERROR)
	{
		printf("\nFailed to close socket: " + WSAGetLastError());
		exit(0);
	}
}

bool Server::SendInt(int id, int value)
{
	int returnCheck = send(mClientConnections[id], (char *)& value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetInt(int id, int& value)
{
	int returnCheck = recv(mClientConnections[id], (char *)& value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendString(int id, const std::string& message)
{
	if (!SendPacketType(id, PACKET::mChatMessage))
		return false;

	int bufferLength = message.size();
	if (!SendInt(id, bufferLength))
		return false;

	int returnCheck = send(mClientConnections[id], message.c_str(), bufferLength, NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetString(int id, std::string& message)
{
	int bufferLength = 0;
	if (!GetInt(id, bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];

	int returnCheck = recv(mClientConnections[id], buffer, bufferLength, NULL);
	buffer[bufferLength] = '\0';
	message = buffer;
	delete[] buffer;

	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetMatch(int id, bool& value)
{
	int returnCheck = send(mClientConnections[id], (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendMatch(int id, bool value)
{
	if (!SendPacketType(id, PACKET::mMatchmakingCheck))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetPlayerType(int id, int& value)
{
	int returnCheck = recv(mClientConnections[id], (char *)& value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendPlayerType(int id, int value)
{
	if (!SendPacketType(id, PACKET::mPlayerType))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)& value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetGameData(int id, GameData& value)
{
	int returnCheck = recv(mClientConnections[id], (char *)& value, sizeof(GameData), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendGameData(int id, GameData value)
{
	if (!SendPacketType(id, PACKET::mData))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)& value, sizeof(GameData), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendPacketType(int id, const PACKET& mPacket)
{
	int returnCheck = send(mClientConnections[id], (char *)& mPacket, sizeof(PACKET), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetPacketType(int id, PACKET& mType)
{
	int returnCheck = recv(mClientConnections[id], (char *)&mType, sizeof(PACKET), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		printf("\nGetting Packet Failed...");
		return false;
	}

	return true;
}

bool Server::ProcessPacket(int index, PACKET mType)
{
	std::string message;
	bool matchmakingPossible = false;
	int playerType;
	GameData mData;

	switch (mType)
	{
	case PACKET::mChatMessage:
		if (!GetString(index, message))
			return false;

		for (int i = 0; i < mConnections; i++)		//For every client connected to the server, send the message
		{
			if (i == index)
				continue;

			//Add user to the start of the message, see in chat
			std::string newMessage = usernames[index] + ": " + message;
			if (!SendString(i, newMessage))
				printf("\nFailed to send message...");
		}

		printf("\nMessage was sent across clients");
		break;

	case PACKET::mMatchmakingCheck:
		//For now default is boolean packet value 
		if (!GetMatch(index, matchmakingPossible))
			return false;

		for (size_t i = 0; i < mClientConnections.size(); i++)
		{
			if (mThreadActive[i] == true)			//Only way to 'disable' removed threads since deleting gives errors
			{
				if (i == index)
					continue;

				if (mClientAvailable.at(i) == true)
				{
					matchmakingPossible = true;
					mMatchups.push_back({ index, i });
					mClientAvailable.at(i) = false;
					mClientAvailable.at(index) = false;

					if (!SendMatch(i, matchmakingPossible) || !SendMatch(index, matchmakingPossible))
					{
						printf("\nFailed to send bool message");
						return false;
					}
					break;
				}
			}
		}

		break;

	case PACKET::Empty:

		break;

	case PACKET::mData:
	{

		if (!GetGameData(index, mData))
			return false;

		for (int i = 0; i < mMatchups.size(); i++)		//Sends turn information to both clients in the match
		{
			if (index == mMatchups[i].first || index == mMatchups[i].second)
			{
				if (!SendGameData(mMatchups[i].first, mData) || !SendGameData(mMatchups[i].second, mData))
					return false;
			}
		}
	}

		break;

	case PACKET::mPlayerType:				//Error when sending this, rarely both exes are set as player 1, no explanation for this

		if (!GetPlayerType(index, playerType))
			return false;


		for (size_t i = 0; i < mMatchups.size(); i++)
		{
			if (index == mMatchups[i].first)
			{
				if (!SendPlayerType(index, 1))
					return false;

				printf("\nThe client at %d is getting set as player 2", index);
				i = mMatchups.size();
			}

			else if(index == mMatchups[i].second)
			{
				if (!SendPlayerType(index, 2))
					return false;

				printf("\nThe client at %d is getting set as player 1", index);
				i = mMatchups.size();		//break out of loop
			}

			printf("\nThe matchup vector for %d is <%d,%d>...", index, mMatchups[0].first, mMatchups[0].second);
		}

		break;

	default:
		//printf("\nUnknown Packet type...");
		//return false;
		break;
	}

	return true;
}

void Server::GetUsername(int index)
{
	serverPtr->usernames.push_back("");

	PACKET mPacket;

	//Get Username from client
	bool usernameSaved = true;

	if (!serverPtr->GetPacketType(index, mPacket))
		printf("\nUnable to obtain client username...");

	if (mPacket == PACKET::mMatchmakingCheck)
		printf("\nObtaining username type not found...");

	std::string username;
	serverPtr->GetString(index, username);

	serverPtr->usernames[index] = username;

	std::cout << "\nUsername of client is: " << username.c_str() << "\n";
}
