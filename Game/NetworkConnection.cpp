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
		exit(0);

	//Create a Socket
	if ((connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		exit(0);

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

	return true;
}

bool NetworkConnection::SendPlayerName(std::string name)
{
	if (!SendString(name))
		return false;

	Sleep(1);

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
			Sleep(1);
			return true;
		}

		current = std::chrono::steady_clock::now();			//Get current time

		if (std::chrono::duration_cast<std::chrono::microseconds>(current - start).count() / 1000000.f > 8)			//If application times out or takes too long
		{
			Sleep(1);
			return false;
		}

	} while (result > 0);

	return result == 0;
}

bool NetworkConnection::CheckForRematch()
{
	int value = 0;

	if (!SendRematch(2))
	{
		CloseConnection();
		return false;
	}

	//Setup timer variables used for while loop
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point current;

	do
	{
		current = std::chrono::steady_clock::now();			//Get current time

		if (!GetRematch(value))
		{
			CloseConnection();
			return false;
		}

		if (value == 2)
			return true;

	} while (std::chrono::duration_cast<std::chrono::microseconds>(current - start).count() / 1000000.f < 8);		//Time out

	SendRematch(0);
	return false;
}

bool NetworkConnection::GetPlayer(int& playerType)		//Error happens here
{
	//Find which player is which (player 1 or 2)
	int result = 0;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point current;

	do
	{
		if (!SendPlayerType(playerType))		//Finds which player they are in the game (1 or 2)
		{
			CloseConnection();
			return false;
		}

		if (!GetPlayerType(playerType))		//Connection error with server, break out
		{
			CloseConnection();
			return false;
		}

		if (playerType == 1 || playerType == 2)			//Found result from server
			result = 1;

		current = std::chrono::steady_clock::now();		//Gets the current time
		if (std::chrono::duration_cast<std::chrono::microseconds>(current - start).count() / 1000000.f > 4)		//Connection time out
		{
			Sleep(1);
			CloseConnection();
			return false;
		}

	} while (result == 0);

	return true;
}

bool NetworkConnection::GetDataUpdate()
{
	GameData newData;		//Assign data from server to local variable to check results before applying to the game

	if (!GetGameData(newData))
		return false;

	VerifyData(newData);

	return true;
}

void NetworkConnection::VerifyData(GameData& ServerData)
{
	if (ServerData.mDisconnected == -1)
		mGameData.mDisconnected = ServerData.mDisconnected;

	if (ServerData.gameEnded)
		mGameData.gameEnded = ServerData.gameEnded;

	if (ServerData.mTurn > Turn::None && ServerData.mTurn < 3)
		mGameData.mTurn = ServerData.mTurn;

	if ((ServerData.mLastMove.first > 0 && ServerData.mLastMove.first < 7) &&
		(ServerData.mLastMove.second > 0 && ServerData.mLastMove.second < 8))
	{
		mGameData.mLastMove = ServerData.mLastMove;
	}

	if (ServerData.mMessage.size() > 3 && ServerData.mMessage.find(':') != std::string::npos/*&& ServerData.mMessage[0] > NULL*/)		//Prevents null messages and '.' messages from showing, obtained during non-blocking data
	{
		Sleep(1);
		mGameData.mMessage = ServerData.mMessage;
	}

	if (ServerData.mWinMessage.size() > 1 && ServerData.mWinMessage[0] > NULL)		//Prevents null messages from being shown
	{
		Sleep(1);
		mGameData.mWinMessage = ServerData.mWinMessage;
	}

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

	return true;
}

bool NetworkConnection::SendMatch(const bool& value)
{
	if (!SendPacketType(PACKET::mMatchmakingCheck))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetRematch(int& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;

		value = -1;
	}

	return true;
}

bool NetworkConnection::SendRematch(const int value)
{
	if (!SendPacketType(PACKET::mRematch))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetGameData(GameData& value)
{
	char data[GAMEDATA_SIZE];
	int returnCheck = recv(connectSocket, data, GAMEDATA_SIZE, NULL);

	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;

		Sleep(1);
		value.mMessage = "";
	}
	else
		DeserializeStruct(&value, data);

	return true;
}

bool NetworkConnection::SendGameData(GameData& value)
{
	if (!SendPacketType(PACKET::mData))
		return false;

	char data[GAMEDATA_SIZE];

	SerializeStruct(&value, data);

	int returnCheck = send(connectSocket, (char *)&data, GAMEDATA_SIZE, NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	Sleep(1);		//Delays game to allow time for data to be passed

	return true;
}

bool NetworkConnection::GetPlayerType(int& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;

		Sleep(1);
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

	Sleep(1);

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

	//Disconnection value
	data[i] = mPacket->mDisconnected;
	i++;

	//Game Ended value
	data[i] = mPacket->gameEnded;
	i++;

	//Turn value
	data[i] = mPacket->mTurn;
	i++;

	//Last move variable stuff
	data[i] = mPacket->mLastMove.first;
	i++;

	data[i] = mPacket->mLastMove.second;
	i++;

	//Chat Message Variable
	size_t j = 0;
	while (j < mPacket->mMessage.size())
	{
		data[i] = mPacket->mMessage[j];
		i++;
		j++;
	}

	data[i] = NULL;		//End of chat message data
	i++;

	//Win Message Variable
	j = 0;
	while (j < mPacket->mWinMessage.size())
	{
		data[i] = mPacket->mWinMessage[j];
		i++;
		j++;
	}

	data[i] = NULL;		//End of win message data
}

void NetworkConnection::DeserializeStruct(GameData* mPacket, char* data)
{
	int i = 0;

	//Disconnection Variable
	mPacket->mDisconnected = data[i];
	i++;

	//Game End Variable
	if (data[i] != 1)
		mPacket->gameEnded = false;
	else
		mPacket->gameEnded = true;

	i++;

	//Turn Variable
	mPacket->mTurn = (Turn)data[i];
	i++;

	//Last move variable stuff
	mPacket->mLastMove.first = (int)data[i];
	i++;

	mPacket->mLastMove.second = (int)data[i];
	i++;

	//Chat Variable
	std::string newMessage;
	int j = 0;
	while (data[i] > NULL && j < 30)
	{
		newMessage.push_back(data[i]);
		i++;
		j++;
	}

	mPacket->mMessage = newMessage;
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

	mPacket->mWinMessage = newWinMessage;
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
	//Let server know this client is disconnecting
	GameData closingData;
	closingData.mDisconnected = -1;
	SendGameData(closingData);

	closesocket(connectSocket);
	WSACleanup();
}