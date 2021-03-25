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
	}

	printf("\nLost Connection with User: %d", index);
	serverPtr->CloseConnection(index);
	serverPtr->mConnections--;
	serverPtr->usernames.erase(serverPtr->usernames.begin() + index);
	serverPtr->mClientConnections.erase(serverPtr->mClientConnections.begin() + index);
	serverPtr->mConnectionThreads[index].detach();
	serverPtr->mConnectionThreads.erase(serverPtr->mConnectionThreads.begin() + index);
	//Debug out messages
	printf("\nConnections on server: %d ", serverPtr->mConnections);
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

bool Server::GetBool(int id, bool& value)
{
	int returnCheck = send(mClientConnections[id], (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendBool(int id, bool value)		//Create own function specifically for matching checks
{
	if (!SendPacketType(id, PACKET::mMatchmakingCheck))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)&value, sizeof(bool), NULL);
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
		if (!GetBool(index, matchmakingPossible))
			return false;

		for (size_t i = 0; i < mClientConnections.size(); i++)
		{
			if (i == index)
				continue;

			//Other user on server, connect them?, store users who are connected together in a vector like structure
			bool found = true;
			if (!SendBool(index, found))
			{
				printf("\nFailed to send bool message");
				return false;
			}
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
