#include "Server.h"

#define PORT 8888		//Port the connection is over

int main()
{
	Server mServer(PORT);

	for (int i = 0; i < 100; i++)
		mServer.ListenForNewConnections();		//Adds a client when they attempt to connect, up to 100 clients allowed

	system("pause");
	return EXIT_SUCCESS;
}