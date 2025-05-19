#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <compare>
#include <iosfwd>
#include <cstdint>
#include <string>
#include <vector>

namespace lab2 {

class BigInt {
public:
	BigInt();
	BigInt(long long value);
	explicit BigInt(const std::string& str);

	// Для тестов: получить строковое представление
	std::string str() const;

	// Утилитарные методы (ранее были private)
	// Возвращает 10^power
	static BigInt base_pow(size_t power);
	// Остаток по 10^power (нижние power цифр)
	BigInt mod_base_power(size_t power) const;
	// Отбросить младшие power цифр
	BigInt shift_right(size_t power) const;

	// ввод/вывод
	friend std::ostream& operator<<(std::ostream&, const BigInt&);
	friend std::istream& operator>>(std::istream&, BigInt&);

	// сравнение
	bool operator==(const BigInt& rhs) const;
	bool operator!=(const BigInt& rhs) const;
	std::strong_ordering operator<=>(const BigInt& rhs) const;

	// арифметика
	BigInt operator+(const BigInt& rhs) const;
	BigInt operator-(const BigInt& rhs) const;
	BigInt operator*(const BigInt& rhs) const;
	BigInt operator/(const BigInt& rhs) const;
	BigInt operator%(const BigInt& rhs) const;

private:
	std::vector<uint8_t> digits_;  // младший разряд в [0]
	bool is_negative_ = false;

	// базовые операции над модулями
	void trim();
	static int  compare_abs(const BigInt& a, const BigInt& b);
	static BigInt add_abs  (const BigInt& a, const BigInt& b);
	static BigInt sub_abs  (const BigInt& a, const BigInt& b);
};

} // namespace lab2

#endif // BIG_INT_HPP
