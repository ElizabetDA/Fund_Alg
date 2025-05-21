#ifndef GAME_HPP
#define GAME_HPP

#include <stdexcept>
#include <algorithm>

class Game {
public:
    // Инициализируем 21 палочкой
    explicit Game(int total = 21) : total_(total) {}

    // Делает ход: сначала вычитает take, потом ход сервера.
    // Возвращает, сколько взял сервер:
    //   >0 — нормальный ход сервера,
    //    0 — клиент выиграл (взял последнюю),
    //   -1 — ошибка (invalid move).
    int play(int take) {
        if (take < 1 || take > 3 || take > total_)
            return -1;                   // неверный ход

        // клиент взял палочки
        total_ -= take;
        if (total_ <= 0) {
            // клиент взял последнюю — он выиграл
            return 0;
        }

        // ход сервера: стратегически забираем так, чтобы (total_ % 4 == 0)
        int serv = total_ % 4;
        if (serv == 0) serv = 1;         // если уже кратно 4, берём 1
        serv = std::clamp(serv, 1, total_);
        total_ -= serv;
        return serv;
    }

    bool isFinished() const {
        return total_ <= 0;
    }

private:
    int total_;
};

#endif // GAME_HPP
