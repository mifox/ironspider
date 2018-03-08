#include <string>
#include <map>
#ifdef WIN32

#else
#define __stdcall 
#endif

class Socket;

class webserver {
public:
	struct http_request {

		http_request() : authentication_given_(false) {}

		Socket*                            s_;
		std::string                        method_;
		std::string                        path_;
		std::map<std::string, std::string> params_;
		std::string                        accept_;
		std::string                        accept_language_;
		std::string                        accept_encoding_;
		std::string                        user_agent_;
		std::string                        status_;
		std::string                        auth_realm_;
		std::string                        answer_;
		bool authentication_given_;
		std::string username_;
		std::string password_;
	};

	typedef   void (*request_func) (http_request*);
	webserver(unsigned int port_to_listen, request_func);


private:
	static unsigned __stdcall Request(void*);
	static request_func request_func_;
};
