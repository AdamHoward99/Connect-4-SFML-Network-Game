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
		CloseConnection();

	return true;
}

bool NetworkConnection::SendPlayerName(std::string name)
{
	if (!SendString(name))
		return false;

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
			result = -1;

		if (opponentFound)		//If found an opponent
			result = 0;

		current = std::chrono::steady_clock::now();			//Get current time

		if (std::chrono::duration_cast<std::chrono::microseconds>(current - start).count() / 1000000.f > 8)			//If application times out or takes too long
			result = -1;

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

bool NetworkConnection::GetTurnUpdate(Turn& mTurn)
{
	GameData newTurn;

	if (!GetGameData(newTurn))
		return false;

	if (newTurn.mTurn == Turn::Player_1_Turn || newTurn.mTurn == Turn::Player_2_Turn)		//If is a valid turn value
		mTurn = newTurn.mTurn;

	return true;
}

bool NetworkConnection::GetMatch(bool& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)		//Error expected due to time constraints of connection (Non blocking mode only)
			return false;
	}

	return true;
}

bool NetworkConnection::SendMatch(const int& value)
{
	if (!SendPacketType(PACKET::mMatchmakingCheck))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetGameData(GameData& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(GameData), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			return false;
	}

	return true;
}

bool NetworkConnection::SendGameData(const GameData& value)
{
	if (!SendPacketType(PACKET::mData))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(GameData), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

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

	return true;
}

bool NetworkConnection::SendString(const std::string& message)
{
	if (!SendPacketType(PACKET::mChatMessage))
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

void NetworkConnection::SerializeStruct(GameData mPacket, std::ostream& os)
{
	//Serialize GameData struct into a stringstream
	char end = '\0';
	char inbetween = '/';

	//os.write((char *)mPacket.mTurn, sizeof(Turn));
	//os.write(&inbetween, 1);		//Separates data in struct
	//os.write(&end, 1);		//End the data stream
}

void NetworkConnection::DeserializeStruct(GameData& mPacket, std::istream is)
{

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
	ioctlsocket(connectSocket, FIONBIO, 0);

	closesocket(connectSocket);
	WSACleanup();

}