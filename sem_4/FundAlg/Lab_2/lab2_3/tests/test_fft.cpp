// tests/test_big_int_extra.cpp

#include <gtest/gtest.h>
#include "../include/big_int.hpp"
#include <sstream>
#include <random>
#include <stdexcept>
#include <string>

using lab2::BigInt;

// Хелпер для получения строки из BigInt
static std::string toString(const BigInt& x) {
	std::ostringstream ss;
	ss << x;
	return ss.str();
}


// --- Конструкторы ---
TEST(Constructor, DefaultZero) {
    BigInt a;
    EXPECT_EQ(toString(a), "0");
    EXPECT_EQ(a, BigInt("0"));
}

TEST(Constructor, LongLong) {
    BigInt a(123456789LL);
    EXPECT_EQ(toString(a), "123456789");
    BigInt b(-987654321LL);
    EXPECT_EQ(toString(b), "-987654321");
}

TEST(Constructor, StringValid) {
    EXPECT_EQ(toString(BigInt("0")), "0");
    EXPECT_EQ(toString(BigInt("+0")), "0");
    EXPECT_EQ(toString(BigInt("-0")), "0");
    EXPECT_EQ(toString(BigInt("000123")), "123");
    EXPECT_EQ(toString(BigInt("-000123")), "-123");
    EXPECT_EQ(toString(BigInt("+456789")), "456789");
    EXPECT_EQ(toString(BigInt("-42")), "-42");
}

TEST(Constructor, StringInvalid) {
    EXPECT_THROW(BigInt(""), std::invalid_argument);
    EXPECT_THROW(BigInt("+"), std::invalid_argument);
    EXPECT_THROW(BigInt("-"), std::invalid_argument);
    EXPECT_THROW(BigInt("12a3"), std::invalid_argument);
    EXPECT_THROW(BigInt(" 123"), std::invalid_argument);
    EXPECT_THROW(BigInt("123 "), std::invalid_argument);
}

// --- Ввод/вывод ---
TEST(IO, StreamInOut) {
    std::istringstream bad(" abc ");
    BigInt x;
    EXPECT_THROW(bad >> x, std::invalid_argument);

    std::istringstream good(" -00099");
    BigInt y;
    good >> y;
    EXPECT_EQ(toString(y), "-99");

    std::ostringstream out;
    out << y;
    EXPECT_EQ(out.str(), "-99");
}

// --- Сложение ---
TEST(Addition, Basic) {
    EXPECT_EQ(BigInt("0") + BigInt("0"), BigInt("0"));
    EXPECT_EQ(BigInt("0") + BigInt("123"), BigInt("123"));
    EXPECT_EQ(BigInt("123") + BigInt("0"), BigInt("123"));
}
TEST(Addition, Positive) {
    EXPECT_EQ(BigInt("123") + BigInt("456"), BigInt("579"));
    EXPECT_EQ(BigInt("999") + BigInt("1"), BigInt("1000"));
}
TEST(Addition, NegativePositive) {
    EXPECT_EQ(BigInt("-5") + BigInt("3"), BigInt("-2"));
    EXPECT_EQ(BigInt("5") + BigInt("-7"), BigInt("-2"));
    EXPECT_EQ(BigInt("-5") + BigInt("5"), BigInt("0"));
}
TEST(Addition, Negative) {
    EXPECT_EQ(BigInt("-5") + BigInt("-6"), BigInt("-11"));
}

// --- Вычитание ---
TEST(Subtraction, Basic) {
    EXPECT_EQ(BigInt("0") - BigInt("0"), BigInt("0"));
    EXPECT_EQ(BigInt("0") - BigInt("5"), BigInt("-5"));
    EXPECT_EQ(BigInt("5") - BigInt("0"), BigInt("5"));
}
TEST(Subtraction, Positive) {
    EXPECT_EQ(BigInt("1000") - BigInt("1"), BigInt("999"));
    EXPECT_EQ(BigInt("123") - BigInt("23"), BigInt("100"));
    EXPECT_EQ(BigInt("100") - BigInt("100"), BigInt("0"));
}
TEST(Subtraction, NegativePositive) {
    EXPECT_EQ(BigInt("-5") - BigInt("3"), BigInt("-8"));
    EXPECT_EQ(BigInt("5") - BigInt("-3"), BigInt("8"));
}
TEST(Subtraction, Negative) {
    EXPECT_EQ(BigInt("-5") - BigInt("-3"), BigInt("-2"));
}

// --- Деление ---
TEST(Division, Basic) {
    EXPECT_EQ(BigInt("0") / BigInt("5"), BigInt("0"));
    EXPECT_EQ(BigInt("5") / BigInt("1"), BigInt("5"));
}
TEST(Division, Truncate) {
    EXPECT_EQ(BigInt("7") / BigInt("3"), BigInt("2"));
    EXPECT_EQ(BigInt("-7") / BigInt("3"), BigInt("-2"));
    EXPECT_EQ(BigInt("7") / BigInt("-3"), BigInt("-2"));
    EXPECT_EQ(BigInt("-7") / BigInt("-3"), BigInt("2"));
}
TEST(Division, DivideByZero) {
    EXPECT_THROW(BigInt("1") / BigInt("0"), std::domain_error);
}

// --- Сравнения ---
TEST(Comparison, Operators) {
    EXPECT_TRUE(BigInt("1") < BigInt("2"));
    EXPECT_FALSE(BigInt("2") < BigInt("1"));
    EXPECT_TRUE(BigInt("-1") < BigInt("0"));
    EXPECT_TRUE(BigInt("-2") < BigInt("-1"));

    EXPECT_TRUE(BigInt("1") <= BigInt("1"));
    EXPECT_TRUE(BigInt("1") <= BigInt("2"));
    EXPECT_TRUE(BigInt("-2") <= BigInt("-1"));

    EXPECT_TRUE(BigInt("2") > BigInt("1"));
    EXPECT_TRUE(BigInt("0") > BigInt("-1"));

    EXPECT_TRUE(BigInt("2") >= BigInt("2"));
    EXPECT_TRUE(BigInt("3") >= BigInt("2"));

    EXPECT_TRUE(BigInt("5") == BigInt("5"));
    EXPECT_TRUE(BigInt("5") != BigInt("6"));
}

// --- Большие числа ---
TEST(LargeNumbers, Addition) {
    std::string a(500, '1'), b(500, '1');
    std::string expected(500, '2');
    EXPECT_EQ(BigInt(a) + BigInt(b), BigInt(expected));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
