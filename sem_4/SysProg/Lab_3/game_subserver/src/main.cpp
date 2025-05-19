#include "../include/MsgQueue.hpp"
#include "../include/Game.hpp"
#include <iostream>

int main() {
	MsgQueue mq(ftok(".", 'M'));
	while (true) {
		// mtype=0 — забираем любую сессию
		GameMsg in = mq.recv(0);
		try {
			Game g;
			int srv = g.play(in.take);
			GameMsg out{in.mtype, srv};
			mq.send(out);
		} catch (const std::exception& ex) {
			GameMsg out{ in.mtype, -1 };
			mq.send(out);
			std::cerr << "Game error: " << ex.what() << std::endl;
		}
	}
	return 0;
}
