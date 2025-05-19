#include "../include/big_int.hpp"
#include <stdexcept>

namespace lab2 {

BigInt mod_exp(const BigInt& base_in, const BigInt& exp_in, const BigInt& mod) {
    if (mod == BigInt(0))
        throw std::domain_error("Modulo by zero");

    BigInt result(1);
    BigInt base = base_in % mod;
    BigInt exp = exp_in;
    const BigInt two(2);

    while (exp > BigInt(0)) {
        // Проверяем, нечётная ли текущая степень
        if (exp % two != BigInt(0)) {
            result = (result * base) % mod;
        }
        // Возводим основание в квадрат по модулю
        base = (base * base) % mod;
        // Делим степень на 2
        exp = exp / two;
    }
    return result;
}

} // namespace lab2