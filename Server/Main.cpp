#include "Server.h"

#define PORT 8888

int main()
{
	Server mServer(PORT);

	for (int i = 0; i < 100; i++)
		mServer.ListenForNewConnections();

	system("pause");

	return EXIT_SUCCESS;

	//Sending data

	////Send an initial buffer to client
	//const int recvbuflen = 512;
	//const char *sendbuf = "Test from server to client";
	//char recvbuf[recvbuflen];
	//int sendResult;

	////Receive until the peer shuts down the connection
	//do
	//{
	//	//Accept a client socket
	//	//SOCKET client_socket = accept(listen_socket, nullptr, nullptr);
	//	//if (client_socket == INVALID_SOCKET)
	//	//{
	//	//	printf("accept() failed with error: %d\n", WSAGetLastError());
	//	//	closesocket(listen_socket);
	//	//	WSACleanup();
	//	//	return EXIT_FAILURE;
	//	//}

	//	result = recv(client_socket, recvbuf, recvbuflen, 0);
	//	if (result > 0)
	//	{
	//		printf("\nReceiving message from client was successful...");
	//		printf("\nBytes Received: %d\n", result);

	//		//Echo the buffer back to the sender
	//		sendResult = send(client_socket, recvbuf, result, 0);
	//		if (sendResult == SOCKET_ERROR)
	//		{
	//			printf("\nSend Failed: %d\n", WSAGetLastError());
	//			closesocket(client_socket);
	//			WSACleanup();
	//			return EXIT_FAILURE;
	//		}

	//		printf("\nSending to client was successful...");
	//		printf("\nBytes Sent: %d\n", sendResult);
	//	}
	//	else if (result == 0)
	//		printf("\nConnection Closing...");
	//	else
	//	{
	//		printf("\nrecv failed: %d\n", WSAGetLastError());
	//		closesocket(client_socket);
	//		WSACleanup();
	//		return EXIT_FAILURE;
	//	}
	//} while (result > 0);

	////Shutdown the send half of the connection since no more data will be sent
	//result = shutdown(client_socket, SD_SEND);
	//if (result == SOCKET_ERROR)
	//{
	//	printf("\nShutdown Failed: %d\n", WSAGetLastError());
	//	closesocket(client_socket);
	//	WSACleanup();
	//	return EXIT_FAILURE;
	//}

	////Cleanup
	//printf("\nCleaning up server...");
	//closesocket(client_socket);
	//WSACleanup();

	//return EXIT_SUCCESS;


}