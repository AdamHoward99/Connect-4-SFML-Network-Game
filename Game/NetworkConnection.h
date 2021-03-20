//Prevents Winsock 1.1 from being included

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

class NetworkConnection
{
public:
	NetworkConnection();		//Default constructor
	~NetworkConnection();		//Default destructor

	void CreateSocket();
	void ConnectToServer();
	void SendData();
	void CloseConnection();

private:
	WSAData wsaData;
	SOCKET connectSocket;
	struct sockaddr_in server;

	const char *sendBuffer = "Test Message to server";
	int connectionResult;
};