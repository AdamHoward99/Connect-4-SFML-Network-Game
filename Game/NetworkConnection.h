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
#include "../Server/PACKET.h"

class NetworkConnection
{
public:
	NetworkConnection();		//Default constructor
	~NetworkConnection();		//Default destructor

	//Initial Setup Functions
	bool ConnectToServer();
	void CreateSocket();
	void CloseConnection();

	void SendPlayerName(std::string username);

	void SendData();

	//Boolean Data Functions
	bool GetBool(bool &foo);
	bool SendBool(const int& value) const;

	//String Data Functions
	bool SendString(const std::string& message);
	bool GetString(std::string& message);

	//Int Data Functions
	bool GetInt(int& value);
	bool SendInt(const int& value) const;

	//Packet Data Functions
	bool SendPacketType(const PACKET& mType);

	bool AlreadyConnected() { return ConnectedToServer; }

private:
	WSAData wsaData;
	SOCKET connectSocket;
	struct sockaddr_in server;

	const char *sendBuffer = "Test Message to server";		//Not sure if needed anymore
	int connectionResult;
	bool ConnectedToServer = false;
};