#pragma once

#include <compare>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace lab2 {

class BigInt {
public:
    BigInt();
    explicit BigInt(long long value);
    explicit BigInt(const std::string& str);

    [[nodiscard]] BigInt operator+(const BigInt& rhs) const;
    [[nodiscard]] BigInt operator-(const BigInt& rhs) const;
    [[nodiscard]] BigInt operator*(const BigInt& rhs) const;
    [[nodiscard]] BigInt operator/(const BigInt& rhs) const;

    [[nodiscard]] std::strong_ordering operator<=>(const BigInt& rhs) const;
    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const BigInt& v);
    friend std::istream& operator>>(std::istream& is, BigInt& v);

private:
    std::vector<uint8_t> digits_;
    bool is_negative_{false};

    static BigInt add_abs(const BigInt& a, const BigInt& b);
    static BigInt sub_abs(const BigInt& a, const BigInt& b);
    static int compare_abs(const BigInt& a, const BigInt& b);

    void trim();
};

} // namespace lab2
