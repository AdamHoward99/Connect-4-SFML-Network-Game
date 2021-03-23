//Prevents Winsock 1.1. from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "PACKET.h"

#include <vector>
#include <thread>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>


class Server
{
public:
	Server(int port);			//Default Constructor
	~Server();					//Default Destructor

	void ListenForNewConnections();
	void CloseConnection(int index);

	static void ClientHandler(int index);

	bool ProcessPacket(int id, PACKET packetType);

	bool GetPacketType(int id, PACKET& packetType);
	bool SendPacketType(int id, const PACKET& mPacket);

	bool SendInt(int id, int value);
	bool GetInt(int id, int& value);

	bool SendString(int id, const std::string& message);
	bool GetString(int id, std::string& message);

	bool SendBool(int id, bool value);
	bool GetBool(int id, bool& value);

	void GetUsername(int id);

	std::vector<std::string> usernames = {};

private:
	SOCKET mClientConnections[100];
	std::thread mConnectionThreads[100];
	int mConnections;

	SOCKET listen_socket;
	addrinfo* info;
	addrinfo hints;
	int result;
};

static Server* serverPtr;