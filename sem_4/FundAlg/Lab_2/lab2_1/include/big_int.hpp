#pragma once

#include <compare>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace lab2 {

class BigInt {
public:
	BigInt();
	BigInt(long long value);
	explicit BigInt(const std::string& str);

	BigInt(const BigInt&) = default;
	BigInt(BigInt&&) noexcept = default;
	BigInt& operator=(const BigInt&) = default;
	BigInt& operator=(BigInt&&) noexcept = default;
	~BigInt() = default;

	[[nodiscard]] BigInt operator+(const BigInt& rhs) const;
	[[nodiscard]] BigInt operator-(const BigInt& rhs) const;
	[[nodiscard]] BigInt operator*(const BigInt& rhs) const;
	[[nodiscard]] BigInt operator/(const BigInt& rhs) const;

	std::strong_ordering operator<=>(const BigInt& rhs) const;
	bool operator==(const BigInt& rhs) const = default;
	bool operator!=(const BigInt& rhs) const { return !(*this == rhs); }
	bool operator< (const BigInt& rhs) const { return (*this <=> rhs) == std::strong_ordering::less; }
	bool operator<=(const BigInt& rhs) const { return (*this <=> rhs) != std::strong_ordering::greater; }
	bool operator> (const BigInt& rhs) const { return (*this <=> rhs) == std::strong_ordering::greater; }
	bool operator>=(const BigInt& rhs) const { return (*this <=> rhs) != std::strong_ordering::less; }


	friend std::ostream& operator<<(std::ostream& os, const BigInt& value);
	friend std::istream& operator>>(std::istream& is, BigInt& value);

private:
	std::vector<uint8_t> digits_;
	bool is_negative_ {false};

	static BigInt add_abs(const BigInt& a, const BigInt& b);
	static BigInt sub_abs(const BigInt& a, const BigInt& b);
	static int compare_abs(const BigInt& a, const BigInt& b);
	void trim();
};

} // namespace lab2
