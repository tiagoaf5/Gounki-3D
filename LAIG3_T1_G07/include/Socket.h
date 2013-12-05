#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <winsock2.h>
#include <iostream>

class Socket
{
private:
	int port;
	char * ip;
	SOCKET m_socket;
public:
	Socket();
	bool socketConnect();
	void sendData(char *s, int len);
	void receiveData(char * ans);
	void quit();
};

#endif