#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "Socket.hpp"

class Client {
public:
	Client(const std::string& address, uint16_t port);
	void run();
private:
	Socket sock_;
};

#endif // CLIENT_HPP
