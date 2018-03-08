/*
 * define file about portable socket class. 
 * description:this sock is suit both windows and linux
 * design:odison
 * e-mail:odison@126.com>
 * 
 */

#ifndef _Socket_H_
#define _Socket_H_
#include <string>
#ifdef WIN32
#include <winsock.h>
typedef int	socklen_t;

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>



typedef int	SOCKET;

	//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
	//#pragma endregion
#endif

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class Socket {

public:
	Socket(SOCKET sock = INVALID_SOCKET);
	~Socket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);
	
	bool Ioctl();

	// Connect socket
	bool Connect(const char* ip, unsigned short port);
//#region server
	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5); 

	// Accept socket
	bool Accept(Socket& s, char* fromip = NULL);
//#endregion
	
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);
	
	std::string ReceiveLine();
	std::string ReceiveBytes();
	
	void SendLine(std::string s);
	void SendBytes(const std::string& s);
	// Close socket
	int Close();

	// Get errno
	int GetError();
	
	//#pragma region just for win32
	// Init winsock DLL 
	static int Init();	
	// Clean winsock DLL
	static int Clean();
	//#pragma endregion

	// Domain parse
	static bool DnsParse(const char* domain, char* ip);

	Socket& operator =(SOCKET s);

	operator SOCKET();

	SOCKET s_;
	
};

class SocketClient : public Socket {
public:
	SocketClient(const std::string& host, int port);
};


class SocketServer : public Socket {
public:
	SocketServer(int port, int connections, TypeSocket type = BlockingSocket);

	Socket* Accept();

};


class SocketSelect {
public:
	//SocketSelect(Socket const * const s1, Socket const * const s2 = NULL, TypeSocket type = BlockingSocket);

	bool Readable(Socket const * const s);

private:
	fd_set fds_;
}; 





#endif
