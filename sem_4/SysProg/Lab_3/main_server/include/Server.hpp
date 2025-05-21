#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include "Socket.hpp"
#include "SharedMemSem.hpp"
#include "MsgQueue.hpp"
#include "Logger.hpp"
#include <thread>
#include <vector>
#include <string>

class Server {
public:
	Server(uint16_t port);
	void run();
private:
	void handleClient(Socket clientSock);

	Socket sock_;
	SharedMemSem shm_;
	MsgQueue mq_;
};

#endif // SERVER_HPP
