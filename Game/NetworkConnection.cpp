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
		exit(0);

	ConnectedToServer = true;

	return true;
}

bool NetworkConnection::SendPlayerName(std::string name)
{
	if (!SendString(name))
		return false;

	return true;
}

void NetworkConnection::SendData()
{

	//Send an initial buffer
	connectionResult = send(connectSocket, sendBuffer ,(int)strlen(sendBuffer), 0);
	if (connectionResult == SOCKET_ERROR)
	{
		CloseConnection();
		assert(EXIT_FAILURE);
	}

}

bool NetworkConnection::GetMatch(bool& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(bool), MSG_PEEK);
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

	int returnCheck = send(connectSocket, (char *)&value, sizeof(bool), MSG_PARTIAL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetPlayerTurn(Turn& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(Turn), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::SendPlayerTurn(const Turn& value)
{
	if (!SendPacketType(PACKET::mPlayerTurn))
		return false;

	int returnCheck = send(connectSocket, (char *)&value, sizeof(Turn), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkConnection::GetPlayerType(int& value)
{
	int returnCheck = recv(connectSocket, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

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