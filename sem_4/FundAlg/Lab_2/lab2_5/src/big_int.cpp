#include "../include/big_int.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <complex>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <sstream>

namespace {

// базовая точка для FFT
using cd = std::complex<double>;
const double PI = std::acos(-1);

void fft(std::vector<cd>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j |= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(std::cos(ang), std::sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len/2; ++j) {
                cd u = a[i+j];
                cd v = a[i+j+len/2] * w;
                a[i+j]       = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (auto& x : a)
            x /= n;
    }
}

// свёртка через FFT, возвращает длинный вектор
std::vector<long long> multiply_fft(const std::vector<int>& a,
                                    const std::vector<int>& b) {
    int n = 1;
    while (n < int(a.size() + b.size()))
        n <<= 1;
    std::vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i)
        fa[i] *= fb[i];
    fft(fa, true);

    std::vector<long long> res(n);
    for (int i = 0; i < n; ++i)
        res[i] = std::llround(fa[i].real());
    return res;
}

} // namespace

namespace lab2 {

//получить строку
std::string BigInt::str() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

//ведущие нули
void BigInt::trim() {
    while (!digits_.empty() && digits_.back() == 0)
        digits_.pop_back();
    if (digits_.empty())
        is_negative_ = false;
}

// Сравнение модулей: -1, 0, +1
int BigInt::compare_abs(const BigInt& a, const BigInt& b) {
    if (a.digits_.size() != b.digits_.size())
        return a.digits_.size() < b.digits_.size() ? -1 : +1;
    for (int i = int(a.digits_.size()) - 1; i >= 0; --i) {
        if (a.digits_[i] != b.digits_[i])
            return a.digits_[i] < b.digits_[i] ? -1 : +1;
    }
    return 0;
}

// Сложение модулей (без учёта знака)
BigInt BigInt::add_abs(const BigInt& a, const BigInt& b) {
    BigInt r;
    size_t n = std::max(a.digits_.size(), b.digits_.size());
    r.digits_.assign(n + 1, 0);
    uint8_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint8_t sum = carry;
        if (i < a.digits_.size()) sum += a.digits_[i];
        if (i < b.digits_.size()) sum += b.digits_[i];
        r.digits_[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) r.digits_[n] = carry;
    r.trim();
    return r;
}

// Вычитание модулей (a >= b по модулю)
BigInt BigInt::sub_abs(const BigInt& a, const BigInt& b) {
    BigInt r;
    r.digits_.assign(a.digits_.size(), 0);
    int8_t borrow = 0;
    for (size_t i = 0; i < a.digits_.size(); ++i) {
        int8_t diff = int8_t(a.digits_[i])
                    - int8_t(i < b.digits_.size() ? b.digits_[i] : 0)
                    - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        r.digits_[i] = uint8_t(diff);
    }
    r.trim();
    return r;
}

BigInt::BigInt() = default;

BigInt::BigInt(long long value) {
    if (value < 0) {
        is_negative_ = true;
        value = -value;
    }
    if (value == 0) return;
    while (value > 0) {
        digits_.push_back(uint8_t(value % 10));
        value /= 10;
    }
}

BigInt::BigInt(const std::string& s) {
    if (s.empty())
        throw std::invalid_argument("BigInt: empty string");
    size_t pos = 0;
    if (s[0] == '+' || s[0] == '-') {
        is_negative_ = (s[0] == '-');
        pos = 1;
    }
    if (pos == s.size())
        throw std::invalid_argument("BigInt: no digits");
    for (size_t i = s.size(); i-- > pos; ) {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            throw std::invalid_argument("BigInt: invalid char");
        digits_.push_back(uint8_t(s[i] - '0'));
    }
    trim();
}

std::ostream& operator<<(std::ostream& os, const BigInt& v) {
    if (v.digits_.empty()) {
        os << '0';
        return os;
    }
    if (v.is_negative_)
        os << '-';
    for (auto it = v.digits_.rbegin(); it != v.digits_.rend(); ++it)
        os << char('0' + *it);
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& v) {
    std::string s;
    if (!(is >> s))
        throw std::runtime_error("BigInt: input error");
    v = BigInt(s);
    return is;
}

bool BigInt::operator==(const BigInt& rhs) const {
    return (*this <=> rhs) == 0;
}

bool BigInt::operator!=(const BigInt& rhs) const {
    return !(*this == rhs);
}

std::strong_ordering BigInt::operator<=>(const BigInt& rhs) const {
    if (is_negative_ != rhs.is_negative_)
        return is_negative_
            ? std::strong_ordering::less
            : std::strong_ordering::greater;
    int c = compare_abs(*this, rhs);
    if (c == 0) return std::strong_ordering::equal;
    bool less = (c < 0);
    return is_negative_
        ? (less ? std::strong_ordering::greater
                : std::strong_ordering::less)
        : (less ? std::strong_ordering::less
                : std::strong_ordering::greater);
}

BigInt BigInt::operator+(const BigInt& rhs) const {
    if (is_negative_ == rhs.is_negative_) {
        BigInt r = add_abs(*this, rhs);
        r.is_negative_ = is_negative_;
        return r;
    }
    int c = compare_abs(*this, rhs);
    if (c == 0)
        return BigInt();
    if (c > 0) {
        BigInt r = sub_abs(*this, rhs);
        r.is_negative_ = is_negative_;
        return r;
    } else {
        BigInt r = sub_abs(rhs, *this);
        r.is_negative_ = rhs.is_negative_;
        return r;
    }
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    BigInt tmp = rhs;
    tmp.is_negative_ = !tmp.is_negative_;
    return *this + tmp;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    // быстрый порог для schoolbook
    constexpr size_t SCHOOLBOOK_THRESHOLD = 64;

    if (digits_.empty() || rhs.digits_.empty())
        return BigInt();

    if (digits_.size() < SCHOOLBOOK_THRESHOLD
     && rhs.digits_.size() < SCHOOLBOOK_THRESHOLD) {
        // наивное умножение
        BigInt r;
        r.is_negative_ = is_negative_ ^ rhs.is_negative_;
        size_t n = digits_.size(), m = rhs.digits_.size();
        r.digits_.assign(n + m, 0);

        for (size_t i = 0; i < n; ++i) {
            uint16_t carry = 0;
            for (size_t j = 0; j < m || carry; ++j) {
                uint32_t cur = r.digits_[i + j]
                             + uint32_t(digits_[i])
                             * uint32_t(j < m ? rhs.digits_[j] : 0)
                             + carry;
                r.digits_[i + j] = uint8_t(cur % 10);
                carry = uint16_t(cur / 10);
            }
        }
        r.trim();
        return r;
    } else {
        // FFT-умножение
        std::vector<int> a(digits_.begin(), digits_.end()),
                         b(rhs.digits_.begin(), rhs.digits_.end());
        auto conv = multiply_fft(a, b);

        BigInt r;
        r.is_negative_ = is_negative_ ^ rhs.is_negative_;
        r.digits_.clear();

        long long carry = 0;
        for (size_t i = 0; i < conv.size(); ++i) {
            long long val = conv[i] + carry;
            r.digits_.push_back(uint8_t(val % 10));
            carry = val / 10;
        }
        while (carry > 0) {
            r.digits_.push_back(uint8_t(carry % 10));
            carry /= 10;
        }
        r.trim();
        return r;
    }
}

// Возвращает 10^power
BigInt BigInt::base_pow(size_t power) {
    BigInt r;
    r.digits_.assign(power, 0);
    r.digits_.push_back(1);
    return r;
}

// Остаток по 10^power (нижние power цифр)
BigInt BigInt::mod_base_power(size_t power) const {
    BigInt r = *this;
    if (r.digits_.size() > power)
        r.digits_.resize(power);
    r.trim();
    return r;
}

// Отбросить младшие power цифр
BigInt BigInt::shift_right(size_t power) const {
    if (digits_.size() <= power)
        return BigInt();
    BigInt r;
    r.digits_.assign(digits_.begin() + power, digits_.end());
    r.is_negative_ = false;
    r.trim();
    return r;
}

// Деление: метод Ньютона + fast path для одного цифрного делителя
BigInt BigInt::operator/(const BigInt& rhs) const {
    if (rhs.digits_.empty())
        throw std::domain_error("Division by zero");
    // если делитель больше по модулю
    if (compare_abs(*this, rhs) < 0)
        return BigInt();

    // fast path: одноцифровый делитель  делим «столбиком»
    if (rhs.digits_.size() == 1) {
        uint8_t d = rhs.digits_[0];
        BigInt q;
    	// знак частного = XOR знаков операндов
        q.is_negative_ = is_negative_ ^ rhs.is_negative_;
        size_t n = digits_.size();
        q.digits_.assign(n, 0);
        uint16_t rem = 0;
        for (size_t i = n; i-- > 0; ) { //от старшего к младшему
            uint16_t cur = rem * 10 + digits_[i];
            q.digits_[i] = uint8_t(cur / d);
            rem = cur % d;
        }
        q.trim();
        return q;
    }

    // общий случай: Ньютона + FFT
    BigInt A = *this; A.is_negative_ = false;
    BigInt B = rhs;   B.is_negative_ = false;

    size_t nA = A.digits_.size();
    size_t nB = B.digits_.size();
    size_t m = nA > nB ? nA - nB : 0;
    size_t k = m + 1;

    // начальное приближение h0 = floor(10 / ведущей цифры B)
    uint8_t lead = B.digits_.back();
    BigInt h = BigInt(10 / lead);

    size_t p = 1;  //точность
    while (p < k) {
        size_t p2 = std::min(p * 2, k);
        BigInt t1    = (B * h).mod_base_power(p2);
        BigInt two10 = base_pow(p) + base_pow(p);
        BigInt u     = two10 - t1;
        h = (h * u).mod_base_power(p2);
        p = p2;
    }

    BigInt Qtilde = (A * h).shift_right(k);
    BigInt one(1);
    while (B * (Qtilde + one) <= A) Qtilde = Qtilde + one;
    while (B * Qtilde         > A) Qtilde = Qtilde - one;

    Qtilde.is_negative_ = (is_negative_ ^ rhs.is_negative_);
    Qtilde.trim();
    return Qtilde;
}

// Остаток от деления
BigInt BigInt::operator%(const BigInt& rhs) const {
    BigInt q = *this / rhs;
    return *this - q * rhs;
}

} // namespace lab2
