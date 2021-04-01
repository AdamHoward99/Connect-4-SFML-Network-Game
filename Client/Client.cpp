#include "Client.h"

Client::Client(int port)
{
	WSADATA wsa_data;
	printf("\nInitializing Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("\nFailed. Error Code: %d", WSAGetLastError());
		exit(0);
	}

	printf("\nInitialized");

	//Create a Socket
	if ((mConnection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		printf("\nCould not create socket : %d", WSAGetLastError());

	printf("\nSocket Created...\n");

	//Connect to server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(mConnection, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		printf("\nConnection to server error");
		closesocket(mConnection);
		WSACleanup();
		exit(0);
	}

	printf("\nSuccessfully connected to the server\n");

	clientPtr = this;
}

Client::~Client()
{
	//Delete things here
}

bool Client::CloseConnection()
{
	if (closesocket(mConnection == SOCKET_ERROR))
	{
		if (WSAGetLastError() == WSAENOTSOCK)		//What?
			return true;

		printf("\nFailed to close socket...");
		return false;
	}

	return true;
}

bool Client::SendString(const std::string& message)
{
	if (!SendPacketType(PACKET::mChatMessage))
		return false;

	int bufferLength = message.size();
	if (!SendInt(bufferLength))
		return false;

	int returnCheck = send(mConnection, message.c_str(), bufferLength, NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::GetString(std::string& message)
{
	int bufferLength = 0;
	if (!GetInt(bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';

	int returnCheck = recv(mConnection, buffer, bufferLength, NULL);
	message = buffer;
	delete[] buffer;

	std::cout << "\n" << message << "\n";

	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::SendInt(const int& value) const
{
	int returnCheck = send(mConnection, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::GetInt(int& value)
{
	int returnCheck = recv(mConnection, (char *)&value, sizeof(int), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;
	
	return true;
}

bool Client::SendBool(bool value)
{

	int returnCheck = send(mConnection, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::GetBool(bool& value)
{
	int returnCheck = recv(mConnection, (char *)&value, sizeof(bool), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::SendPacketType(const PACKET& mPacket)
{
	int returnCheck = send(mConnection, (char *)&mPacket, sizeof(PACKET), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

bool Client::GetPacketType(PACKET& mPacket)
{
	int returnCheck = recv(mConnection, (char *)&mPacket, sizeof(PACKET), NULL);
	if (returnCheck == SOCKET_ERROR)
		return false;

	return true;
}

void Client::ClientHandler()
{
	PACKET mPacket;
	while (true)
	{
		if (!clientPtr->GetPacketType(mPacket))
			break;

		if (!clientPtr->ProcessPacket(mPacket))
			break;
	}

	//Lost Connection
	printf("\nLost connection to server...");
	if (clientPtr->CloseConnection())
		printf("\nSocket to server was closed...");
}

bool Client::ProcessPacket(PACKET mPacketType)
{
	std::string message;

	switch (mPacketType)
	{
	case PACKET::mChatMessage:
		if (!GetString(message))
			return false;

		break;

	default:
		printf("\nUnknown Packet Type");
		break;
	}

	return true;
}
