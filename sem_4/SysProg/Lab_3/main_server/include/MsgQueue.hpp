#ifndef MSGQUEUE_HPP
#define MSGQUEUE_HPP

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>

struct GameMsg {
	long mtype;
	int take;
};

class MsgQueue {
public:
	MsgQueue(key_t key) {
		msqid_ = msgget(key, IPC_CREAT | 0666);
		if (msqid_ == -1) throw std::runtime_error(std::string("msgget failed: ") + std::strerror(errno));
	}

	void send(const GameMsg& msg) {
		if (msgsnd(msqid_, &msg, sizeof(msg.take), 0) == -1)
			throw std::runtime_error(std::string("msgsnd failed: ") + std::strerror(errno));
	}

	GameMsg recv(long mtype) {
		GameMsg msg;
		if (msgrcv(msqid_, &msg, sizeof(msg.take), mtype, 0) == -1)
			throw std::runtime_error(std::string("msgrcv failed: ") + std::strerror(errno));
		return msg;
	}

private:
	int msqid_;
};

#endif // MSGQUEUE_HPP
