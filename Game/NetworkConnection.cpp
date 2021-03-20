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

void NetworkConnection::ConnectToServer()
{
	//Connect to Server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(connectSocket, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		closesocket(connectSocket);
		WSACleanup();
		assert(EXIT_FAILURE);
	}

}

void NetworkConnection::SendData()
{
	//Sends data to server
	const int receiveBufferLength = 512;
	char receiveBuffer[receiveBufferLength];

	//Send an initial buffer
	connectionResult = send(connectSocket, sendBuffer ,(int)strlen(sendBuffer), 0);
	if (connectionResult == SOCKET_ERROR)
	{
		CloseConnection();
		assert(EXIT_FAILURE);
	}

}

void NetworkConnection::CloseConnection()
{
	closesocket(connectSocket);
	WSACleanup();

}