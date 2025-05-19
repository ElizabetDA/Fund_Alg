#include "../include/big_int.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

using lab2::BigInt;

void BigInt::trim() {
    while (!digits_.empty() && digits_.back() == 0)
        digits_.pop_back();
    if (digits_.empty())
        is_negative_ = false;
}

int BigInt::compare_abs(const BigInt& a, const BigInt& b) {
    if (a.digits_.size() != b.digits_.size())
        return a.digits_.size() < b.digits_.size() ? -1 : 1;
    for (int i = static_cast<int>(a.digits_.size()) - 1; i >= 0; --i) {
        if (a.digits_[i] != b.digits_[i])
            return a.digits_[i] < b.digits_[i] ? -1 : 1;
    }
    return 0;
}

BigInt BigInt::add_abs(const BigInt& a, const BigInt& b) {     //+
    BigInt res;
    size_t n = std::max(a.digits_.size(), b.digits_.size());
    res.digits_.assign(n + 1, 0);
    uint8_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint8_t sum = carry;
        if (i < a.digits_.size()) sum += a.digits_[i];
        if (i < b.digits_.size()) sum += b.digits_[i];
        res.digits_[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) res.digits_[n] = carry;
    res.trim();
    return res;
}

BigInt BigInt::sub_abs(const BigInt& a, const BigInt& b) {    //-
    // предполагаем |a| >= |b|
    BigInt res;
    res.digits_.assign(a.digits_.size(), 0);
    int8_t borrow = 0;
    for (size_t i = 0; i < a.digits_.size(); ++i) {
        int8_t diff = a.digits_[i] - (i < b.digits_.size() ? b.digits_[i] : 0) - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else { borrow = 0; }
        res.digits_[i] = static_cast<uint8_t>(diff);
    }
    res.trim();
    return res;
}

BigInt::BigInt() = default;

BigInt::BigInt(long long value) {
    if (value < 0) { is_negative_ = true; value = -value; }
    if (value == 0) return;
    while (value) {
        digits_.push_back(static_cast<uint8_t>(value % 10));
        value /= 10;
    }
}

BigInt::BigInt(const std::string& str) {
    if (str.empty()) throw std::invalid_argument("Empty string");
    size_t pos = 0;
    if (str[0] == '+' || str[0] == '-') {
        is_negative_ = (str[0] == '-');
        pos = 1;
    }
    for (size_t i = str.size(); i-- > pos;) {
        if (!std::isdigit(static_cast<unsigned char>(str[i])))
            throw std::invalid_argument("Invalid digit");
        digits_.push_back(static_cast<uint8_t>(str[i] - '0'));
    }
    trim();
}

namespace lab2 {

std::ostream& operator<<(std::ostream& os, const BigInt& v) {
    if (v.digits_.empty()) { os << '0'; return os; }
    if (v.is_negative_) os << '-';
    for (auto it = v.digits_.rbegin(); it != v.digits_.rend(); ++it)
        os << char('0' + *it);
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& v) {
    std::string s;
    is >> s;
    v = BigInt(s);
    return is;
}

} // namespace lab2

BigInt BigInt::operator+(const BigInt& rhs) const {
    if (is_negative_ == rhs.is_negative_) {
        BigInt sum = add_abs(*this, rhs);
        sum.is_negative_ = is_negative_;
        return sum;
    }
    int cmp = compare_abs(*this, rhs);
    if (cmp == 0) return BigInt();
    if (cmp > 0) {
        BigInt diff = sub_abs(*this, rhs);
        diff.is_negative_ = is_negative_;
        return diff;
    } else {
        BigInt diff = sub_abs(rhs, *this);
        diff.is_negative_ = rhs.is_negative_;
        return diff;
    }
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    BigInt neg = rhs;
    neg.is_negative_ = !rhs.is_negative_;
    return *this + neg;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    // всегда используем Карацубу
    return mul_karatsuba(*this, rhs);
}

BigInt BigInt::operator/(const BigInt& rhs) const {
    if (rhs.digits_.empty())
        throw std::domain_error("Division by zero");
    if (compare_abs(*this, rhs) < 0)
        return BigInt();

    BigInt dividend = *this; dividend.is_negative_ = false;
    BigInt divisor  = rhs;   divisor.is_negative_ = false;

    BigInt result;
    result.digits_.assign(dividend.digits_.size(), 0);

    BigInt current;
    for (int i = static_cast<int>(dividend.digits_.size()) - 1; i >= 0; --i) {
        current.digits_.insert(current.digits_.begin(), dividend.digits_[i]);
        current.trim();

        uint8_t l = 0, r = 10, x = 0;
        while (l <= r) {
            uint8_t m = (l + r) / 2;
            BigInt prod = divisor * BigInt(static_cast<long long>(m));
            if (compare_abs(prod, current) <= 0) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }

        result.digits_[i] = x;
        current = current - (divisor * BigInt(static_cast<long long>(x)));
    }

    result.is_negative_ = is_negative_ ^ rhs.is_negative_;
    result.trim();
    return result;
}

std::strong_ordering BigInt::operator<=>(const BigInt& rhs) const {
    if (is_negative_ != rhs.is_negative_)
        return is_negative_ ? std::strong_ordering::less : std::strong_ordering::greater;
    int cmp = compare_abs(*this, rhs);
    if (cmp == 0) return std::strong_ordering::equal;
    bool less = (cmp < 0);
    return is_negative_
           ? (less ? std::strong_ordering::greater : std::strong_ordering::less)
           : (less ? std::strong_ordering::less   : std::strong_ordering::greater);
}

// рекурсивный Карацуба
BigInt BigInt::mul_karatsuba(const BigInt& x, const BigInt& y) {
    if (x.digits_.empty() || y.digits_.empty())
        return BigInt();
    // базовый случай: один разряд (irjkmysq ckexfq)
    if (x.digits_.size() == 1 || y.digits_.size() == 1) {
        BigInt res;
        res.digits_.assign(x.digits_.size() + y.digits_.size(), 0);
        for (size_t i = 0; i < x.digits_.size(); ++i) {
            uint8_t carry = 0;
            for (size_t j = 0; j < y.digits_.size() || carry; ++j) {
                uint64_t cur = res.digits_[i + j]
                    + static_cast<uint64_t>(x.digits_[i])
                    * (j < y.digits_.size() ? y.digits_[j] : 0)
                    + carry;
                res.digits_[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        res.is_negative_ = x.is_negative_ ^ y.is_negative_;
        res.trim();
        return res;
    }

    size_t n = std::max(x.digits_.size(), y.digits_.size());
    size_t m = n / 2;

    // разделяем числа на low/high
    BigInt low1, high1, low2, high2;
    low1.digits_.assign(x.digits_.begin(),
                       x.digits_.begin() + std::min(x.digits_.size(), m));
    if (x.digits_.size() > m)
        high1.digits_.assign(x.digits_.begin() + m, x.digits_.end());

    low2.digits_.assign(y.digits_.begin(),
                       y.digits_.begin() + std::min(y.digits_.size(), m));
    if (y.digits_.size() > m)
        high2.digits_.assign(y.digits_.begin() + m, y.digits_.end());

    low1.is_negative_ = high1.is_negative_ = false;
    low2.is_negative_ = high2.is_negative_ = false;

    BigInt z0 = mul_karatsuba(low1, low2);
    BigInt z2 = mul_karatsuba(high1, high2);
    BigInt sum1 = add_abs(low1, high1);
    BigInt sum2 = add_abs(low2, high2);
    BigInt z1 = mul_karatsuba(sum1, sum2);
    z1 = sub_abs(sub_abs(z1, z2), z0);

    // собираем ответ: z2 * 10^(2m) + z1 * 10^m + z0
    BigInt result;
    result.digits_ = z0.digits_;

    // прибавляем z1 << m
    if (!z1.digits_.empty()) {
        result.digits_.resize(std::max(result.digits_.size(),
                                       z1.digits_.size() + m), 0);
        for (size_t i = 0; i < z1.digits_.size(); ++i)
            result.digits_[i + m] += z1.digits_[i];
    }
    // прибавляем z2 << (2m)
    if (!z2.digits_.empty()) {
        result.digits_.resize(std::max(result.digits_.size(),
                                       z2.digits_.size() + 2*m), 0);
        for (size_t i = 0; i < z2.digits_.size(); ++i)
            result.digits_[i + 2*m] += z2.digits_[i];
    }
    // нормировка по основанию 10
    uint8_t carry = 0;
    for (size_t i = 0; i < result.digits_.size(); ++i) {
        uint8_t sum = result.digits_[i] + carry;
        result.digits_[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) result.digits_.push_back(carry);

    result.is_negative_ = x.is_negative_ ^ y.is_negative_;
    result.trim();
    return result;
}
