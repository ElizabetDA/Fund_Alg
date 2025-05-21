#include "big_int.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

using lab2::BigInt;

void BigInt::trim() {
    while (!digits_.empty() && digits_.back() == 0)9 digits_.pop_back();
    if (digits_.empty()) is_negative_ = false;
}

int BigInt::compare_abs(const BigInt& a, const BigInt& b) {
    if (a.digits_.size() != b.digits_.size())
        return (a.digits_.size() < b.digits_.size()) ? -1 : 1;
    for (int i = static_cast<int>(a.digits_.size()) - 1; i >= 0; --i) {
        if (a.digits_[i] != b.digits_[i])
            return (a.digits_[i] < b.digits_[i]) ? -1 : 1;
    }
    return 0;
}

BigInt BigInt::add_abs(const BigInt& a, const BigInt& b) {
    BigInt res;
    size_t n = std::max(a.digits_.size(), b.digits_.size());
    res.digits_.resize(n + 1);
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

BigInt BigInt::sub_abs(const BigInt& a, const BigInt& b) {
    BigInt res;
    res.digits_.resize(a.digits_.size());
    int8_t borrow = 0;
    for (size_t i = 0; i < a.digits_.size(); ++i) {
        int8_t diff = a.digits_[i] - (i < b.digits_.size() ? b.digits_[i] : 0) - borrow;
        if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
        res.digits_[i] = static_cast<uint8_t>(diff);
    }
    res.trim();
    return res;
}

BigInt::BigInt() = default;

BigInt::BigInt(long long value) {
    if (value < 0) { is_negative_ = true; value = -value; }
    if (value == 0) return;
    while (value) { digits_.push_back(value % 10); value /= 10; }
}

BigInt::BigInt(const std::string& str) {
    if (str.empty()) throw std::invalid_argument("Empty string");
    size_t pos = (str[0] == '+' || str[0] == '-') ? 1 : 0;
    if (str[0] == '-') is_negative_ = true;
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
	std::string s; is >> s; v = BigInt(s); return is;
}

}

BigInt BigInt::operator+(const BigInt& rhs) const {
    if (is_negative_ == rhs.is_negative_) {
        BigInt res = add_abs(*this, rhs);
        res.is_negative_ = is_negative_;
        return res;
    }
    int cmp = compare_abs(*this, rhs);
    if (cmp == 0) return BigInt();
    if (cmp > 0) {
        BigInt res = sub_abs(*this, rhs);
        res.is_negative_ = is_negative_;
        return res;
    }
    BigInt res = sub_abs(rhs, *this);
    res.is_negative_ = rhs.is_negative_;
    return res;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    BigInt neg_rhs = rhs; neg_rhs.is_negative_ = !rhs.is_negative_;
    return *this + neg_rhs;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    if (digits_.empty() || rhs.digits_.empty()) return BigInt();
    BigInt res;
    res.digits_.assign(digits_.size() + rhs.digits_.size(), 0); //таблица рязрядов
    for (size_t i = 0; i < digits_.size(); ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < rhs.digits_.size() || carry; ++j) {
            uint64_t cur = res.digits_[i + j] +
                static_cast<uint64_t>(digits_[i]) *
                (j < rhs.digits_.size() ? rhs.digits_[j] : 0) + carry;
            res.digits_[i + j] = cur % 10;
            carry = cur / 10;
        }
    }
    res.is_negative_ = is_negative_ ^ rhs.is_negative_;
    res.trim();
    return res;
}

BigInt BigInt::operator/(const BigInt& rhs) const {
    if (rhs.digits_.empty()) throw std::domain_error("Division by zero");
    if (compare_abs(*this, rhs) < 0) return BigInt();
    BigInt dividend = *this; dividend.is_negative_ = false;
    BigInt divisor  =  rhs ; divisor .is_negative_ = false;
    BigInt result;  result.digits_.assign(dividend.digits_.size(), 0);
    BigInt current;
    for (int i = static_cast<int>(dividend.digits_.size()) - 1; i >= 0; --i) {
        current.digits_.insert(current.digits_.begin(), dividend.digits_[i]);
        current.trim();
        uint8_t l = 0, r = 10, x = 0;
        while (l <= r) {
            uint8_t m = (l + r) / 2;
            BigInt prod = divisor * BigInt(static_cast<long long>(m));
            if (compare_abs(prod, current) <= 0) { x = m; l = m + 1; }
            else r = m - 1;
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
    bool less = cmp < 0;
    return is_negative_ ? (less ? std::strong_ordering::greater : std::strong_ordering::less)
                        : (less ? std::strong_ordering::less   : std::strong_ordering::greater);
}
