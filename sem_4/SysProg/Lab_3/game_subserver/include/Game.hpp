#ifndef GAME_HPP
#define GAME_HPP

#include <stdexcept>

class Game {
public:
	Game(int total = 21) : total_(total) {}
	// возвращает, сколько взял сервер
	int play(int take) {
		if (take < 1 || take > 3)
			throw std::runtime_error("Invalid move");
		int server_take = (total_ - take) % 4;
		if (server_take == 0) server_take = 1;
		total_ -= (take + server_take);
		if (total_ <= 0)
			throw std::runtime_error("Game over");
		return server_take;
	}
private:
	int total_;
};

#endif // GAME_HPP
