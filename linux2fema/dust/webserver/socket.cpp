/*
 * Source file about portable socket class. 
 *
 * design:odison
 * e-mail:odison@126.com>
 * 
 */



#include <errno.h>
#include "socket.h"
#include "stdio.h"
#include <iostream>
using namespace std;

#ifdef WIN32
#pragma comment(lib, "wsock32")
#else
#include <fcntl.h>
#include <memory.h>
#endif


Socket::Socket(SOCKET sock)
{
	s_ = sock;
}

Socket::~Socket()
{
}

int Socket::Init()
{
#ifdef WIN32
	/*

	typedef struct WSAData { 
		WORD wVersion;								//winsock version
		WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
		char szDescription[WSADESCRIPTION_LEN+1]; 
		char szSystemStatus[WSASYSSTATUS_LEN+1]; 
		unsigned short iMaxSockets; 
		unsigned short iMaxUdpDg; 
		char FAR * lpVendorInfo; 
	}WSADATA, *LPWSADATA; 
	*/
	
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret) {
		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif
	
	return 0;
}
//this is just for windows
int Socket::Clean()
{
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

Socket& Socket::operator =(SOCKET s)
{
	s_ = s;
	return (*this);
}

Socket::operator SOCKET()
{
	return s_;
}
//create a socket object win/lin is the same
// af:
bool Socket::Create(int af, int type, int protocol)
{
	s_ = socket(af, type, protocol);
	if (s_ == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool Socket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(s_, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool Socket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt =  1;
	if (setsockopt(s_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) 
		return false;

	int ret = bind(s_, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}
//for server
bool Socket::Listen(int backlog)
{
	int ret = listen(s_, backlog);
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool Socket::Accept(Socket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(s_, (struct sockaddr*)&cliaddr, &addrlen);
	if (sock == SOCKET_ERROR) {
		return false;
	}

	s = sock;
	if (fromip != NULL)
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int Socket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;

	while (count < len) {

		bytes = send(s_, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	} 

	return count;
}

int Socket::Recv(char* buf, int len, int flags)
{
	return (recv(s_, buf, len, flags));
}

std::string Socket::ReceiveLine()
{
	std::string ret;
	while (1) {
		char r;

		switch (recv(s_, &r, 1, 0)) {
		case 0: // not connected anymore;
			return "";
		case -1:
			if (errno == EAGAIN) {
				return ret;
			}
			else {
			  // not connected anymore
				return "";
			}
		}

		ret += r;
		if (r == '\n')  return ret;
	}
};

std::string Socket::ReceiveBytes()
{
	std::string ret;
	char buf[1024];
 
	for (;;) {
		u_long arg = 0;
		
		if ( Ioctl()!= 0)
			break;
		if (arg == 0)
			break;
		if (arg > 1024)
			arg = 1024;
		int rv = recv(s_, buf, arg, 0);
		if (rv <= 0)
			break;
		std::string t;
		t.assign(buf, rv);
		ret += t;
	}
 
	return ret;
};

bool Socket::Ioctl()
{
#ifdef WIN32
	u_long arg = 0;
	return ioctlsocket(s_, FIONREAD, &arg);
#else	
	fcntl(s_, F_SETFL, O_NONBLOCK);
#endif
	
}

void Socket::SendLine(std::string s) {
	s += '\n';
	send(s_, s.c_str(), s.length(), 0);
}

void Socket::SendBytes(const std::string& s) {
	send(s_, s.c_str(), s.length(), 0);
}

int Socket::Close()
{
#ifdef WIN32
	return (closesocket(s_));
#else
	return (close(s_));
#endif
}

int Socket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

SocketServer::SocketServer(int port, int connections, TypeSocket type) {
	sockaddr_in sa =  {0};


	sa.sin_family = PF_INET;             
	sa.sin_port = htons(port);          
	s_ = socket(AF_INET, SOCK_STREAM, 0);
	if (s_ == INVALID_SOCKET) {
		throw "INVALID_SOCKET";
	}

	if (type == NonBlockingSocket) {
		u_long arg = 1;
		//ioctlsocket(s_, FIONBIO, &arg);
		Socket(s_).Ioctl();
	}

	  /* bind the socket to the internet address */
	if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		Socket(s_).Close();
		throw "INVALID_SOCKET";
	}
  
	listen(s_, connections);                               
}

Socket* SocketServer::Accept() {
	SOCKET new_sock = accept(s_, 0, 0);
	/*
	if (new_sock == INVALID_SOCKET) {
		
		errno;
		int rc = WSAGetLastError();
		if (rc == WSAEWOULDBLOCK) {
			return 0; // non-blocking call, no request pending
		}
		else {
			throw "Invalid Socket";
		}
	}*/



	Socket* r = new Socket(new_sock);
	return r;
}

SocketClient::SocketClient(const std::string& host, int port)
	: Socket() {
	std::string error;


	hostent *he;
	if ((he = gethostbyname(host.c_str())) == 0) {
		error = strerror(errno);
		throw error;
	}


	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((in_addr *)he->h_addr);
	memset(&(addr.sin_zero), 0, 8); 


	if (::connect(s_, (sockaddr *) &addr, sizeof(sockaddr))) {
	//	error = strerror(WSAGetLastError());
	//	throw error;
	}
}
/*
SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type) {
	FD_ZERO(&fds_);
	FD_SET(const_cast<Socket*>(s1)->s_, &fds_);
	if (s2) {
		FD_SET(const_cast<Socket*>(s2)->s_, &fds_);
	}     


	TIMEVAL tval;
	tval.tv_sec  = 0;
	tval.tv_usec = 1;


	TIMEVAL *ptval;
	if (type == NonBlockingSocket) {
		ptval = &tval;
	}
	else { 
		ptval = 0;
	}


	if (select(0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) 
	    == SOCKET_ERROR) throw "Error in select";
}*/

bool SocketSelect::Readable(Socket const * const s) {
	if (FD_ISSET(s->s_, &fds_)) return true;
	return false;
}

bool Socket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;
		
	sprintf(ip, "%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0], 
		(unsigned char)p->h_addr_list[0][1], 
		(unsigned char)p->h_addr_list[0][2], 
		(unsigned char)p->h_addr_list[0][3]);
	
	return true;
}

