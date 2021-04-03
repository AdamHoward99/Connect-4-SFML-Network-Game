#include "NetworkConnection.h"

NetworkConnection::NetworkConnection()
{

}

NetworkConnection::~NetworkConnection()
{
	//Delete things here
}

void NetworkConnection::CreateSocket()
{
	//Winsock connection to server
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		assert(EXIT_FAILURE);

	//Create a Socket
	if ((connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		assert(EXIT_FAILURE);

}

bool NetworkConnection::ConnectToServer()
{
	//Returns true if everything was setup and connected, returns false if anything messed up

	//Winsock connection to server
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	//Create the socket
	if ((connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return false;

	//Connect to the server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(connectSocket, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		closesocket(connectSocket);
		WSACleanup();
		return false;
	}

	//Disables blocking for send, recv etc.
	u_long mode = 1;
	if (ioctlsocket(connectSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		CloseConnection();
		return false;
	}

	OutputDebugStringA("\nConnection was made to the server...");

	return true;
}

bool NetworkConnection::SendPlayerName(std::string name)
{
	if (!SendString(name))
		return false;

	OutputDebugStringA("\nPlayer name was sent to the server...");

	return true;
}

bool NetworkConnection::Matchmake()
{
	bool opponentFound = false;

	if (!SendMatch(opponentFound))
		return false;

	int result = 1;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point current;

	do
	{
		if (!GetMatch(opponentFound))		//Break out if cannot get a value from the server
			return false;

		if (opponentFound)		//If found an opponent
		{
			OutputDebugStringA("\nFound an opponent, returning true...");
			return true;
		}

		current = std::chrono::steady_clock::now();			//Get current time

		if (std::chrono::duration_cast<std::chrono::microseconds>(current - start).count() / 1000000.f > 8)			//If application times out or takes too long
		{
			OutputDebugStringA("\nTimed out...");
			return false;
		}

	} while (result > 0);

	return result == 0;
}

bool NetworkConnection::GetPlayer(int& playerType)
{
	//Find which player is which (player 1 or 2)
	int result = 0;

	if (!SendPlayerType(playerType))		//Finds which player they are in the game (1 or 2)
	{
		CloseConnection();
		return false;
	}

	do
	{
		if (!GetPlayerType(playerType))		//Connection error with server, break out
		{
			CloseConnection();
			return false;
		}

		if (playerType == 1 || playerType == 2)			//Found result from server
			result = 1;

	} while (result == 0);

	return true;
}

bool NetworkConnection::GetDataUpdate(GameData& mData)
{
	GameData newData;		//Assign data from server to local variable to check results before applying to the game

	if (!GetGameData(newData))
		return false;

	if (newData.mTurn != Turn::None)		//If is a valid turn value
	{
		OutputDebugStringA("\nA valid value for the turn has been found...");
		mData.mTurn = newData.mTurn;
	}

	if (newData.mLastMove != std::pair<int, int>{-1, -1})
	{
		OutputDebugStringA("\nA valid vallue for the last move has been found...");
		mData.mLastMove = newData.mLastMove;
	}

	return true;
}

bool NetworkConnection::GetMatch(bool& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)		//Error expected due to time constraints of connection (Non blocking mode only)
			return false;

		value = false;
	}

	OutputDebugStringA("\nGetting the match from the server happened...");

	return true;
}

bool NetworkConnection::SendMatch(const int& value)
{
	if (!SendPacketType(PACKET::mMatchmakingCheck))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	OutputDebugStringA("\nSent searching for match to the server");

	return true;
}

bool NetworkConnection::GetGameData(GameData& value)
{
	char data[GAMEDATA_SIZE];
	int returnCheck = recv(connectSocket, (char *) &data, sizeof(data), NULL);

	DeserializeStruct(&value, data);

	if (returnCheck == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			return false;

		OutputDebugStringA("\nGamedata has returned null...");
		value.mTurn = Turn::None;
		value.mLastMove = { -1, -1 };
	}

	return true;
}

bool NetworkConnection::SendGameData(GameData& value)
{
	if (!SendPacketType(PACKET::mData))
		return false;

	char data[GAMEDATA_SIZE];

	SerializeStruct(&value, data);

	int returnCheck = send(connectSocket, (char *) &data, sizeof(data), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	OutputDebugStringA("\nThe turn has been sent to the server");

	return true;
}

bool NetworkConnection::GetPlayerType(int& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;

		value = 0;
	}

	return true;
}

bool NetworkConnection::SendPlayerType(const int& value)
{
	if (!SendPacketType(PACKET::mPlayerType))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	OutputDebugStringA("\nPlayer type was sent to the server...");

	return true;
}

bool NetworkConnection::SendString(const std::string& message)
{
	if (!SendPacketType(PACKET::mUsername))
		return false;

	int bufferLength = message.size();
	if (!SendInt(bufferLength))
		return false;

	int returnCheck = send(connectSocket, message.c_str(), bufferLength, NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetString(std::string& message)
{
	int bufferLength = 0;
	if (!GetInt(bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';

	int returnCheck = recv(connectSocket, buffer, bufferLength, NULL);
	message = buffer;
	delete[] buffer;

	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetInt(int& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::SendInt(const int& value) const
{
	int returnCheck = send(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

void NetworkConnection::SerializeStruct(GameData* mPacket, char *data)
{
	//Serialize GameData struct
	int i = 0;
	data[i] = mPacket->mTurn;
	i++;

	//Last move variable stuff
	data[i] = mPacket->mLastMove.first;
	i++;

	data[i] = mPacket->mLastMove.second;
	i++;
}

void NetworkConnection::DeserializeStruct(GameData* mPacket, char* data)
{
	int i = 0;

	mPacket->mTurn = (Turn)data[i];
	i++;

	//Last move variable stuff
	mPacket->mLastMove.first = (int)data[i];
	i++;

	mPacket->mLastMove.second = (int)data[i];
	i++;
}

bool NetworkConnection::SendPacketType(const PACKET& mPacket)
{
	int returnCheck = send(connectSocket, (char *)&mPacket, sizeof(PACKET), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

void NetworkConnection::CloseConnection()
{
	closesocket(connectSocket);
	WSACleanup();
	exit(0);		//Debug only

}