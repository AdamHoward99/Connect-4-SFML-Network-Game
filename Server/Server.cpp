#include "Server.h"

Server::Server(int port)
{
	//Setup Network Variables
	WSADATA wsa_data;

	mConnectionResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (mConnectionResult != 0)		//When it fails to obtain the Winsock DLL
	{
		printf("Obtaining Winsock DLL failed with %d...", mConnectionResult);
		WSACleanup();
		exit(0);
	}

	mAddressHints =
	{
		AI_PASSIVE,
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	};

	//Resolve the local address and port to be used by the server
	mConnectionResult = getaddrinfo(nullptr, "8888", &mAddressHints, &mAddressInfo);

	if (mConnectionResult != 0)		//When it fails to obtain the port info
	{
		printf("getaddrinfo() failed: %d\n", mConnectionResult);
		WSACleanup();
		exit(0);
	}

	printf("Successfully opened the port...");

	//Create server socket
	mListenSocket = socket(mAddressInfo->ai_family, mAddressInfo->ai_socktype, mAddressInfo->ai_protocol);

	if (mListenSocket == INVALID_SOCKET)		//When it fails to create the socket
	{
		printf("Unable to create server socket: %d\n", WSAGetLastError());
		freeaddrinfo(mAddressInfo);
		WSACleanup();
		exit(0);
	}

	printf("\nThe server socket was succesfully created...");

	//Setup the TCP listening socket
	mConnectionResult = bind(mListenSocket, mAddressInfo->ai_addr, (int)mAddressInfo->ai_addrlen);

	if (mConnectionResult == SOCKET_ERROR)		//When it fails to setup the socket
	{
		printf("bind() failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(mAddressInfo);
		closesocket(mListenSocket);
		WSACleanup();
		exit(0);
	}
	else
		freeaddrinfo(mAddressInfo);

	printf("\nThe listening socket was successfully setup...");

	//Listen for a single connection
	mConnectionResult = listen(mListenSocket, 1);

	if (mConnectionResult == SOCKET_ERROR)
	{
		printf("listen() failed with error: %d\n", WSAGetLastError());
		closesocket(mListenSocket);
		WSACleanup();
		exit(0);
	}

	mServerPtr = this;
}

Server::~Server()
{
	//Release Server Pointer
	mServerPtr = nullptr;
	delete mServerPtr;
}

void Server::ListenForNewConnections()
{
	//Accept a client socket
	SOCKET client_socket = accept(mListenSocket, nullptr, nullptr);
	if (client_socket == INVALID_SOCKET)
	{
		printf("accept() failed with error: %d\n", WSAGetLastError());
		closesocket(mListenSocket);
		WSACleanup();
		exit(0);
	}
	else
	{
		printf("\nA client has been connected...");
		mClientConnections.push_back(client_socket);
		mThreadActive.push_back(true);
		GetUsername(mConnections);

		mConnectionThreads.push_back(std::thread(ClientHandler, mConnections));
		mConnections++;
	}

}

void Server::ClientHandler(int index)
{
	//Packet
	PACKET mPacketType;

	while (true)		//Loops until the client or server lose connection
	{
		//Receive Messages
		if (!mServerPtr->GetPacketType(index, mPacketType))
			break;

		//Send Messages
		if (!mServerPtr->ProcessPacket(index, mPacketType))
			break;

	}

	//Client Disconnect
	printf("\nLost Connection with User: %d", index);
	mServerPtr->CloseConnection(index);
	mServerPtr->mConnectionThreads[index].detach();
	mServerPtr->mThreadActive[index] = false;

	printf("\nLifetime Connections on server: %d ", mServerPtr->mConnections);
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
	//Get Length of string being sent
	int bufferLength = 0;
	if (!GetInt(id, bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];

	int returnCheck = recv(mClientConnections[id], buffer, bufferLength, NULL);
	buffer[bufferLength] = '\0';		//Detect end of string character
	message = buffer;
	delete[] buffer;

	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Server::GetMatch(int id, bool& value)
{
	int returnCheck = recv(mClientConnections[id], (char *)&value, sizeof(bool), NULL);
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

bool Server::SendPlayerType(int id, int value)
{
	if (!SendPacketType(id, PACKET::mPlayerType))
		return false;

	int returnCheck = send(mClientConnections[id], (char *)& value, sizeof(int), NULL);
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

bool Server::SendGameData(int id, GameData* value)
{
	char data[GAMEDATA_SIZE];

	SerializeStruct(value, data);

	int returnCheck = send(mClientConnections[id], (char *)&data, GAMEDATA_SIZE, NULL);

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
	while (data[i] != NULL && data != "" && j < 30)		//30 is max message size (6 for username, 20 for message), prevents stack cookie error
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
	while (data[i] != NULL && data != "" && j < 15)
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
	//Local Variables to receive data from client
	bool matchmakingPossible = false;
	int playerType;
	GameData mData;
	int rematchPossible = 1;

	switch (mType)
	{
	case PACKET::mMatchmakingCheck:
		if (!GetMatch(index, matchmakingPossible))
			return false;

		for (size_t i = 0; i < mClientConnections.size(); i++)		//Look through every client connected to the server
		{
			if (mThreadActive[i] == true)			//Check if client is still active
			{
				if (i == index)
					continue;

				if (!MatchupExists(i))			//Other client is not currently in another match
				{
					//Create match and rematch in case that is done
					matchmakingPossible = true;
					mMatchups.push_back({ index, i });
					mRematchAccepted.push_back({ 0,0 });

					if (!SendMatch(i, matchmakingPossible) || !SendMatch(index, matchmakingPossible))		//Send matchmaking result to both clients
					{
						printf("\nFailed to send match message");
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
		if (!GetGameData(index, mData))
			return false;

		if (mData.mDisconnected == -1)		//Client is disconnecting from the server
		{
			DeleteMatchup(index);
			return false;
		}

		if (mData.gameEnded)		//Detect that the game is over
			printf("\nThe game has ended, client %d has won...", index);

		for (size_t i = 0; i < mMatchups.size(); i++)		//Sends turn and board information to other client in the match
		{
			if (index == mMatchups[i].first)
			{
				if (!SendGameData(mMatchups[i].second, &mData))
					return false;

				i = mMatchups.size();
			}

			else if (index == mMatchups[i].second)
			{
				if (!SendGameData(mMatchups[i].first, &mData))
					return false;

				i = mMatchups.size();
			}
		}

		break;

	case PACKET::mPlayerType:

		if (!GetInt(index, playerType))
			return false;

		if (playerType == 1 || playerType == 2)		//Already have a valid player type value
		{
			SendPlayerType(index, playerType);
			return true;
		}

		for (size_t i = 0; i < mMatchups.size(); i++)		//Sends player type based on which position they are in the matchup pair
		{
			if (index == mMatchups[i].first)
			{
				if (!SendPlayerType(index, 1))
					return false;

				i = mMatchups.size();
			}

			else if (index == mMatchups[i].second)
			{
				if (!SendPlayerType(index, 2))
					return false;

				i = mMatchups.size();		//break out of loop
			}

		}

		break;

	case PACKET::mRematch:

		if (!GetInt(index, rematchPossible))
			return false;

		if (!MatchupExists(index))
			return false;

		for (size_t i = 0; i < mMatchups.size(); i++)		//Checks if both clients in the matchup pair have requested a rematch
		{
			if (index == mMatchups[i].first)
			{
				mRematchAccepted[i].first = rematchPossible;
				if (mRematchAccepted[i].first == -1 && mRematchAccepted[i].second == -1 && mThreadActive[mMatchups[i].second])
				{
					if (!SendRematch(mMatchups[i].second, rematchPossible) || !SendRematch(index, rematchPossible))		//Sends rematch result to both clients
						return false;

					mRematchAccepted[i] = { 0,0 };		//Reset for future rematches if any
				}

				i = mMatchups.size();
			}

			else if (index == mMatchups[i].second)
			{
				mRematchAccepted[i].second = rematchPossible;
				if (mRematchAccepted[i].second == -1 && mRematchAccepted[i].first == -1 && mThreadActive[mMatchups[i].first])
				{
					if (!SendRematch(mMatchups[i].first, rematchPossible) || !SendRematch(index, rematchPossible))
						return false;

					mRematchAccepted[i] = { 0,0 };	//Reset for future rematches if any
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
	mServerPtr->mUsernames.push_back("");

	PACKET mPacket;

	if (!mServerPtr->GetPacketType(index, mPacket))
		printf("\nUnable to obtain client username...");

	std::string username;
	mServerPtr->GetString(index, username);

	mServerPtr->mUsernames[index] = username;		//Store username of client

	printf("\nUsername of client is: %s", username.c_str());
}

bool Server::MatchupExists(int index)		//Checks if client is currently in a match
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
			mThreadActive[mMatchups[i].second] = false;				//Prevents other client on win menu from matchmaking with other clients
			SendGameData(mMatchups[i].second, &closingData);		//Other user disconnected, send disconnection to this client
			mMatchups.erase(mMatchups.begin() + i);
			mRematchAccepted.erase(mRematchAccepted.begin() + i);
			break;
		}

		else if (index == mMatchups[i].second)
		{
			mThreadActive[mMatchups[i].first] = false;				//Prevents other client on win menu from matchmaking with other clients
			SendGameData(mMatchups[i].first, &closingData);			//Other user disconnected, send disconnection to this client
			mMatchups.erase(mMatchups.begin() + i);
			mRematchAccepted.erase(mRematchAccepted.begin() + i);
			break;
		}
	}

}