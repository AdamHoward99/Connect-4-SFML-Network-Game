//Prevents winsock 1.1 from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameBoard.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>

#define GAME_WIDTH 900
#define GAME_HEIGHT 800

int main()
{
	sf::RenderWindow window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Connect 4", sf::Style::Titlebar | sf::Style::Close);

	//Winsock connection to server
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		return EXIT_FAILURE;

	//Create a Socket
	SOCKET s;

	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return EXIT_FAILURE;

	//Connect to Server
	struct sockaddr_in server;
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		closesocket(s);
		WSACleanup();
		return EXIT_FAILURE;
	}

	//Sends data to server
	const int recvbuflen = 512;
	const char *sendbuf = "Test from SFML Connect 4 game";
	char recvbuf[recvbuflen];

	int result;

	//Send an initial buffer
	result = send(s, sendbuf, (int)strlen(sendbuf), 0);
	if (result == SOCKET_ERROR)
	{
		closesocket(s);
		WSACleanup();
		return EXIT_FAILURE;
	}

	closesocket(s);
	WSACleanup();

	//FPS
	window.setFramerateLimit(60);

	//Instantiate things here
	Game mGame(window);

	while (window.isOpen())
	{
		//Update stuff here
		mGame.Update();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased)
				mGame.MouseReleased(event);
			else if (event.type == sf::Event::TextEntered)
				mGame.KeyPressed(event);

		}

		window.clear();

		//Draw stuff to the screen here, after .clear
		mGame.Draw();

		window.display();
	}

	return 0;
}