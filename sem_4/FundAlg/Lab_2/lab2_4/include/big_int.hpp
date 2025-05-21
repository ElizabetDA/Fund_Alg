#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <vector>
#include <string>
#include <ostream>
#include <compare>
#include <cstdint>

namespace lab2 {

class BigInt {
public:
	BigInt();
	BigInt(long long value);
	BigInt(const std::string& str);

	friend std::ostream& operator<<(std::ostream& os, const BigInt& v);
	friend std::istream& operator>>(std::istream& is, BigInt& v);

	BigInt operator+(const BigInt& rhs) const;
	BigInt operator-(const BigInt& rhs) const;
	BigInt operator*(const BigInt& rhs) const;
	BigInt operator/(const BigInt& rhs) const;

	std::strong_ordering operator<=>(const BigInt& rhs) const;
	bool operator==(const BigInt& rhs) const {
		return (*this <=> rhs) == 0;
	}

private:
	std::vector<uint8_t> digits_;  // младшие ― в начале
	bool is_negative_ = false;

	void trim();  // убирает ведущие нули

	// вспомогательные по модулю
	static int compare_abs(const BigInt& a, const BigInt& b);
	static BigInt add_abs(const BigInt& a, const BigInt& b);
	static BigInt sub_abs(const BigInt& a, const BigInt& b);

	static BigInt mul_karatsuba(const BigInt& x, const BigInt& y);
};

} // namespace lab2

#endif // BIG_INT_HPP
