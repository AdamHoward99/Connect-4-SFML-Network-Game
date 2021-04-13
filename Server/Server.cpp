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

	}

	printf("\nLost Connection with User: %d", index);
	serverPtr->CloseConnection(index);
	serverPtr->mConnectionThreads[index].detach();
	serverPtr->mThreadActive[index] = false;

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

bool Server::GetRematch(int id, int& value)
{
	int returnCheck = recv(mClientConnections[id], (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::SendRematch(int id, int value)
{
	if (!SendPacketType(id, PACKET::mRematch))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetGameData(int id, GameData& value)
{
	char data[GAMEDATA_SIZE];
	int returnCheck = recv(mClientConnections[id], data, GAMEDATA_SIZE, NULL);

	printf("\nObtained %d bytes from game", returnCheck);

	if (returnCheck == SOCKET_ERROR)
		return false;

	DeserializeStruct(&value, data);

	return true;
}

bool Server::SendGameData(int id, GameData value)
{
	if (!SendPacketType(id, PACKET::mData))
		return false;

	char data[GAMEDATA_SIZE];

	SerializeStruct(&value, data);

	int returnCheck = send(mClientConnections[id], (char *) &data, GAMEDATA_SIZE, NULL);

	printf("\nSending %d bytes to game", returnCheck);

	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

void Server::SerializeStruct(GameData* mData, char *data)
{
	int i = 0;

	//Disconnection Variable
	data[i] = mData->mDisconnected;
	i++;

	//Game Ended Variable
	data[i] = mData->gameEnded;
	i++;

	//Turn Variable
	data[i] = mData->mTurn;
	i++;

	//Last move variable stuff
	data[i] = mData->mLastMove.first;
	i++;

	data[i] = mData->mLastMove.second;
	i++;

	//Chat Message Variable
	size_t j = 0;
	while (j < mData->mMessage.size() && mData->mMessage[j] != NULL)
	{
		data[i] = mData->mMessage[j];
		i++;
		j++;
	}

	data[i] = NULL;
	i++;

	//Win Message Variable
	j = 0;
	while (j < mData->mWinMessage.size() && mData->mWinMessage[j] != NULL)
	{
		data[i] = mData->mWinMessage[j];
		i++;
		j++;
	}

	data[i] = NULL;
	i++;

}

void Server::DeserializeStruct(GameData* mData, char *data)
{
	int i = 0;
	//Disconnection Variable
	mData->mDisconnected = data[i];
	i++;

	//Game Ended Variable
	mData->gameEnded = data[i];
	i++;

	//Turn Variable
	mData->mTurn = (Turn)data[i];		//Gets first byte of char data, corresponds to int of Turn enum
	i++;

	//Last move variable stuff
	mData->mLastMove.first = (int)data[i];
	i++;

	mData->mLastMove.second = (int)data[i];
	i++;

	//Chat Variable
	std::string newMessage;
	int j = 0;
	while (data[i] != NULL && data != "" && j < 23)		//23 is max message size (6 for username, 15 for message), prevents stack cookie error
	{
		newMessage.push_back(data[i]);
		i++;
		j++;
	}

	mData->mMessage = newMessage;
	i++;

	//Win Message Variable
	std::string newWinMessage;
	j = 0;
	while (data[i] != NULL && data != "" && j < 15)		//15 max maybe, need to check this
	{
		newWinMessage.push_back(data[i]);
		i++;
		j++;
	}

	mData->mWinMessage = newWinMessage;
	i++;
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
	bool matchmakingPossible = false;
	int playerType;
	GameData mData;
	int rematchPossible = 5;

	switch (mType)
	{
	case PACKET::mMatchmakingCheck:
		if (!GetMatch(index, matchmakingPossible))
			return false;

		for (size_t i = 0; i < mClientConnections.size(); i++)
		{
			if (mThreadActive[i] == true)			//Only way to 'disable' removed threads since deleting gives errors
			{
				if (i == index)
					continue;

				if (!MatchupExists(i))			//Other client is not currently in another match
				{
					matchmakingPossible = true;
					mMatchups.push_back({ index, i });

					if (!SendMatch(i, matchmakingPossible) || !SendMatch(index, matchmakingPossible))
					{
						printf("\nFailed to send bool message");
						return false;
					}

					printf("\nMessages were sent to %d and %d", i, index);
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

		if (mData.mDisconnected == -1)		//Client is disconnecting from the server
		{
			DeleteMatchup(index);
			return false;
		}

		if (mData.gameEnded)		//Detect that the game is over
			printf("\nThe game has ended, client %d has won...", index);

		for (size_t i = 0; i < mMatchups.size(); i++)		//Sends turn information to other client in the match
		{
			if (index == mMatchups[i].first)
			{
				if (!SendGameData(mMatchups[i].second, mData))
					return false;

				i = mMatchups.size();
			}

			else if (index == mMatchups[i].second)
			{
				if (!SendGameData(mMatchups[i].first, mData))
					return false;
			
				i = mMatchups.size();
			}
		}
	}

		break;

	case PACKET::mPlayerType:

		if (!GetPlayerType(index, playerType))
			return false;

		if (playerType == 1 || playerType == 2)
		{
			printf("\nThis client already has obtained a valid player type...");
			SendPlayerType(index, playerType);	
			return true;
		}

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

		}

		break;

	case PACKET::mRematch:
		
		if (!GetRematch(index, rematchPossible))
			return false;

		printf("\nThe client at %d has sent a value of %d", index, rematchPossible);

		for (size_t i = 0; i < mMatchups.size(); i++)
		{
			if (index == mMatchups[i].first)
			{
				rematchAccepted.first = rematchPossible;
				if (rematchAccepted.second == 1)
				{
					if (!SendRematch(mMatchups[i].second, rematchPossible) || !SendRematch(index, rematchPossible))
						return false;
				}

				i = mMatchups.size();
			}

			else if (index == mMatchups[i].second)
			{
				rematchAccepted.second = rematchPossible;
				if (rematchAccepted.first == 1)
				{
					if (!SendRematch(mMatchups[i].first, rematchPossible) || !SendRematch(index, rematchPossible))
						return false;
				}

				i = mMatchups.size();		//break out of loop
			}

		}
		break;

	default:
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

	std::string username;
	serverPtr->GetString(index, username);

	serverPtr->usernames[index] = username;

	std::cout << "\nUsername of client is: " << username.c_str() << "\n";
}

bool Server::MatchupExists(int index)
{
	for (size_t i = 0; i < mMatchups.size(); i++)
	{
		if (index == mMatchups[i].first)
			return true;

		else if (index == mMatchups[i].second)
			return true;
	}

	return false;
}

void Server::DeleteMatchup(int index)
{
	GameData closingData;
	closingData.mDisconnected = -1;

	for (size_t i = 0; i < mMatchups.size(); i++)
	{
		if (index == mMatchups[i].first)
		{
			SendGameData(mMatchups[i].second, closingData);
			mThreadActive.at(mMatchups[i].second) = true;
			mMatchups.erase(mMatchups.begin() + i);
			break;
		}

		else if (index == mMatchups[i].second)
		{
			SendGameData(mMatchups[i].first, closingData);
			mThreadActive.at(mMatchups[i].first) = true;
			mMatchups.erase(mMatchups.begin() + i);
			break;
		}
	}

}

