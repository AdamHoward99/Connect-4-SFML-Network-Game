#include "Client.h"

Client* mClient;

#define PORT 8888

void GetUsername()
{
	bool usernameAccepted = false;

	do
	{
		printf("\nPlease enter a username: ");
		std::string username;
		std::getline(std::cin, username);
		mClient->SendString(username);
		usernameAccepted = true;

	} while (!usernameAccepted);

}

int main()
{
	mClient = new Client(PORT);

	GetUsername();

	mClient->StartRoutine();

	std::string buffer;
	while (true)
	{
		//getline
		std::getline(std::cin, buffer);

		if (!mClient->SendString(buffer))
			break;
	}

	system("pause");

	WSACleanup();
	return 0;


	////Sending data to the server
	//const int recvbuflen = 512;
	//const char *sendbuf = "this is a test";
	//char recvbuf[recvbuflen];

	//int result;

	////Send an initial buffer
	//result = send(s, sendbuf, (int)strlen(sendbuf), 0);
	//if (result == SOCKET_ERROR)
	//{
	//	printf("\nSend Failed: %d\n", WSAGetLastError());
	//	closesocket(s);
	//	WSACleanup();
	//	return EXIT_FAILURE;
	//}

	//printf("\nSending the Buffer was successful...");
	//printf("\nBytes Sent: %d\n", result);

	////Shutdown the connection for sending since no more data will be sent,
	////the client can still use the s socket for receiving data
	//result = shutdown(s, SD_SEND);
	//if (result == SOCKET_ERROR)
	//{
	//	printf("\nShutdown Failed: %d\n", WSAGetLastError());
	//	closesocket(s);
	//	WSACleanup();
	//	return EXIT_FAILURE;
	//}

	////Receive data until the server closes the connection
	//do
	//{
	//	result = recv(s, recvbuf, recvbuflen, 0);
	//	if (result > 0)
	//	{
	//		printf("\nReceived data successfully...");
	//		printf("\nBytes Received: %d\n", result);
	//		//printf("\nMessage received from the server: ", recvbuflen);		//Find way to output the message passed between client and server?
	//	}
	//	else if (result == 0)
	//		printf("\nConnection Closed...");
	//	else
	//		printf("\nRecv failed: %d\n", WSAGetLastError());
	//} while (result > 0);

	//closesocket(s);
	//WSACleanup();
	//return EXIT_SUCCESS;
}