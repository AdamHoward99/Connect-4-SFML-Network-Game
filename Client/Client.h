//Prevents winsock 1.1 from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "../Server/PACKET.h"

#include <thread>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

class Client
{
public:
	Client(int port);			//Default constructor
	~Client();					//Default Destructor

	bool CloseConnection();

	bool SendString(const std::string& message);
	bool GetString(std::string& message);

	bool SendInt(const int& value) const;
	bool GetInt(int& value);

	bool SendPacketType(const PACKET& mType);
	bool GetPacketType(PACKET& mType);

	bool SendBool(bool value);
	bool GetBool(bool& value);

	static void ClientHandler();

	bool ProcessPacket(PACKET mPacketType);

	inline void StartRoutine() { clientThread = std::thread(ClientHandler); }

private:
	SOCKET mConnection;
	struct sockaddr_in server;

	int result;
	std::thread clientThread;

};

static Client* clientPtr;