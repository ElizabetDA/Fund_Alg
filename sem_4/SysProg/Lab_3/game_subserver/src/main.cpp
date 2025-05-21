#include "../include/MsgQueue.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <unordered_map>

int main() {
	// Ключ «M» должен совпадать с тем, что в Server.cpp
	MsgQueue mq(ftok(".", 'M'));
	// Храним состояния по sessionId (mtype)
	std::unordered_map<long, Game> sessions;

	while (true) {
		// Принимаем любой тип mtype (0 значит «любой»)
		GameMsg in = mq.recv(0);
		long sid = in.mtype;
		int take = in.take;

		// Заводим новую сессию, если не было
		auto it = sessions.find(sid);
		if (it == sessions.end()) {
			sessions.emplace(sid, Game(21));
			it = sessions.find(sid);
			std::cerr << "[Game] New session " << sid << "\n";
		}

		Game& g = it->second;
		int result = g.play(take);
		std::cerr << "[Game] session="<< sid
				  << " you="<< take
				  << " srv="<< result
				  << "\n";

		// Если клиент выиграл (result==0) или ошибка (result<0), завершаем сессию
		if (result == 0 || result < 0) {
			sessions.erase(sid);
			std::cerr << "[Game] end session=" << sid
					  << " result=" << result << "\n";
		}

		// Отправляем обратно подслеперу ответ
		GameMsg out{ sid, result };
		mq.send(out);
	}
	return 0;
}
