#include "Socket.h"

using namespace std;

#define IPADDRESS "127.0.0.1"
#define PORT 60070

Socket::Socket()
{
	cout << "Inicializando socket\n";
	bool value = socketConnect();
	if (value)
	{/*
		char *s = "comando(1, 2).\n";
		sendData(s, strlen(s));
		char ans[128];
		receiveData(ans);*/
	}
	//quit();
	//getchar();
}

bool Socket::socketConnect()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
	else
		printf("Client: WSAStartup() is OK.\n");

	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	else
		printf("Client: socket() is OK.\n");

	// Connect to a server.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	// Just test using the localhost, you can try other IP address
	clientService.sin_addr.s_addr = inet_addr(IPADDRESS);
	clientService.sin_port = htons(PORT);
	
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Client: connect() - Failed to connect.\n");
		WSACleanup();
		return false;
	}
	else {
		printf("Client: connect() is OK.\n");
		printf("Client: Can start sending and receiving data...\n");
	}

	// Send and receive data.
	printf("Connected\n");
	return true;
}

void Socket::sendData(char *s, int len)
{
	int bytesSent = send(m_socket, s, len, 0);
	if(bytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
}

void Socket::receiveData(char * ans)
{
	int bytesRecv = SOCKET_ERROR;
	int pos = 0;
	while (true) {
		recv(m_socket, &ans[pos], 1, 0);
		if (ans[pos] == '\n')
			break;
		pos++;
	}
	ans[pos-1] = '\0';
	cout << "prolog answered: " << ans << endl;
}

void Socket::quit()
{
	cout << "Asking prolog to quit" << endl;
	char buff[] = "quit.\n";
	sendData(buff, 6);
	char ans[128];
	receiveData(ans);	
}