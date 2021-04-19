//Prevents Winsock 1.1. from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "PACKET.h"
#include "../Game/GameData.h"

#include <vector>
#include <thread>
#include <cassert>

class Server
{
public:
	Server(int port);			//Default Constructor
	~Server();					//Default Destructor

	void ListenForNewConnections();		//Adds a new client when they are attempting to connect to the server
	void CloseConnection(int index);	//Disconnects client from server

	static void ClientHandler(int index);		//Loops to obtain and send data from the clients thread, stops looping when a disconnect happens

	bool ProcessPacket(int id, PACKET packetType);		//Deciphering type of data received from client

	//Packet Functions
	bool GetPacketType(int id, PACKET& packetType);
	bool SendPacketType(int id, const PACKET& mPacket);

	//Integer Value Functions
	bool SendInt(int id, int value);
	bool GetInt(int id, int& value);

	//String Value Functions
	bool SendString(int id, const std::string& message);
	bool GetString(int id, std::string& message);

	//Matchmaking Functions, is true when 2 clients are both searching for an opponent
	bool SendMatch(int id, bool value);
	bool GetMatch(int id, bool& value);

	//Player Type Function, doesn't need to get as client doesn't decide player type
	bool SendPlayerType(int id, int value);

	//GameData Functions
	bool SendGameData(int id, GameData* value);
	bool GetGameData(int id, GameData& value);

	//Rematch Function
	bool SendRematch(int id, int value);

	//Serialize and Deserialize Functions, used for sending GameData struct values all at once
	void SerializeStruct(GameData* mData, char *data);
	void DeserializeStruct(GameData* mData, char *data);

	//Opponent Disconnect Functions
	bool MatchupExists(int id);		//Checks that match hasnt been removed due to disconnect
	void DeleteMatchup(int id);		//Deletes matchup from vector when one client disconnects

	void GetUsername(int id);
private:
	std::vector<SOCKET> mClientConnections;				//Stores all sockets clients are connected on
	std::vector<std::thread> mConnectionThreads;		//Gives each client a thread to send/receive data simultaneously
	std::vector<std::string> mUsernames = {};			//Stores all usernames of clients connecting to the server
	std::vector<std::pair<int, int>> mMatchups;			//Stores indexes of client threads for client matches, deletes when one of the clients disconnects
	std::vector<std::pair<int, int>> mRematchAccepted;	//Stores values which change to 1 when that client is requesting a rematch, aligns with matchup vector

	std::vector<bool> mThreadActive;					//Detect if client has disconnected, is true if user is still connected to server

	int mConnections = 0;								//Stores total amount of clients which have connected to server (including ones who have disconnected)

	SOCKET mListenSocket;		//Socket that listens for client connections
	addrinfo* mAddressInfo;
	addrinfo mAddressHints;
	int mConnectionResult;
};

static Server* mServerPtr;