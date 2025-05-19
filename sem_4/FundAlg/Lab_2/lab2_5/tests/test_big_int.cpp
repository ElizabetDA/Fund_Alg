#include "big_int.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>

using lab2::BigInt;

// Вспомогательная для создания BigInt из строки
static BigInt BI(const std::string& s) {
    return BigInt(s);
}

// =======================================================
//         Тесты IO, конструкторов и исключений
// =======================================================

TEST(BigIntIO, DefaultAndZero) {
    EXPECT_EQ(BI("0").str(), "0");
    BigInt a;
    EXPECT_EQ(a.str(), "0");
}

TEST(BigIntIO, FromStringPositive) {
    EXPECT_EQ(BI("1234567890").str(), "1234567890");
}

TEST(BigIntIO, FromStringNegative) {
    EXPECT_EQ(BI("-987654321").str(), "-987654321");
}

TEST(BigIntIO, LeadingZeros) {
    EXPECT_EQ(BI("000123").str(), "123");
    EXPECT_EQ(BI("0000").str(), "0");
}

TEST(BigIntIO, StreamExtraction) {
    std::istringstream is("   -42  ");
    BigInt a;
    is >> a;
    EXPECT_EQ(a.str(), "-42");
}

TEST(BigIntIO, StreamExtractionInvalid) {
    std::istringstream is("abc");
    BigInt x;
    EXPECT_THROW(is >> x, std::invalid_argument);
}

TEST(BigIntIO, InvalidStringThrows) {
    EXPECT_THROW(BigInt(""), std::invalid_argument);
    EXPECT_THROW(BigInt("abc123"), std::invalid_argument);
    EXPECT_THROW(BigInt("+"), std::invalid_argument);
}

// =======================================================
//              Тесты сравнения
// =======================================================

TEST(BigIntCmp, Basic) {
    EXPECT_TRUE(BI("0") == BI("0"));
    EXPECT_TRUE(BI("123") < BI("456"));
    EXPECT_TRUE(BI("-5") < BI("3"));
    EXPECT_TRUE(BI("-100") < BI("-2"));
    EXPECT_TRUE(BI("1000") > BI("999"));
}

// =======================================================
//             Тесты сложения/вычитания
// =======================================================

TEST(BigIntAddSub, Positive) {
    EXPECT_EQ((BI("123") + BI("877")).str(), "1000");
    EXPECT_EQ((BI("1000") - BI("1")).str(), "999");
}

TEST(BigIntAddSub, WithSigns) {
    EXPECT_EQ((BI("-123") + BI("200")).str(), "77");
    EXPECT_EQ((BI("123") + BI("-200")).str(), "-77");
    EXPECT_EQ((BI("-500") - BI("200")).str(), "-700");
    EXPECT_EQ((BI("-500") - BI("-200")).str(), "-300");
}

// =======================================================
//                Тесты умножения
// =======================================================

TEST(BigIntMul, SchoolbookSmall) {
    EXPECT_EQ((BI("12") * BI("34")).str(), "408");
    EXPECT_EQ((BI("-12") * BI("34")).str(), "-408");
    EXPECT_EQ((BI("-12") * BI("-34")).str(), "408");
    EXPECT_EQ((BI("0") * BI("123456")).str(), "0");
}

TEST(BigIntMul, FFTThresholdExact) {
    // 10^63 * 10^63 = 10^126
    std::string s = "1" + std::string(63, '0');
    BigInt a = BI(s), b = BI(s);
    std::string expect = "1" + std::string(126, '0');
    EXPECT_EQ((a * b).str(), expect);
}

TEST(BigIntMul, FFTLarge) {
    // 10^64 * 10^64 = 10^128
    std::string t = "1" + std::string(64, '0');
    BigInt x = BI(t), y = BI(t);
    std::string exp2 = "1" + std::string(128, '0');
    EXPECT_EQ((x * y).str(), exp2);
}

TEST(BigIntMul, Large) {
    EXPECT_EQ(
      (BI("12345678901234567890") * BI("98765432109876543210")).str(),
      "1219326311370217952237463801111263526900"
    );
}

// =======================================================
//        Тесты деления (fast path и Newton)
// =======================================================

TEST(BigIntDivMod, SingleDigitFast) {
    EXPECT_EQ((BI("100") / BI("5")).str(), "20");
    EXPECT_EQ((BI("103") / BI("5")).str(), "20");
    EXPECT_EQ((BI("103") % BI("5")).str(), "3");

    // большой делимый / одноцифровый
    std::string n = "1" + std::string(100, '3');
    BigInt big = BI(n);
    BigInt q   = big / BI("7");
    BigInt r   = big % BI("7");
    EXPECT_EQ((q * BI("7") + r).str(), big.str());
}

TEST(BigIntDivMod, DivByZeroThrows) {
    EXPECT_THROW(BI("123") / BI("0"), std::domain_error);
}

// =======================================================
//      Тесты утилитарных приватных методов
//          base_pow, mod_base_power, shift_right
// =======================================================

TEST(BigIntUtils, BasePow) {
    EXPECT_EQ(BigInt::base_pow(0).str(), "1");
    EXPECT_EQ(BigInt::base_pow(1).str(), "10");
    EXPECT_EQ(BigInt::base_pow(5).str(), "100000");
}

TEST(BigIntUtils, ModBasePowerAndShiftRight) {
    BigInt x = BI("123456789");
    EXPECT_EQ(x.mod_base_power(2).str(), "89");
    EXPECT_EQ(x.mod_base_power(5).str(), "56789");
    EXPECT_EQ(x.shift_right(2).str(), "1234567");
    EXPECT_EQ(x.shift_right(8).str(), "1");
    EXPECT_EQ(x.shift_right(9).str(), "0");  // всё отбросили
}

// =======================================================
//             Смешанные цепочки операций
// =======================================================

TEST(BigIntMisc, ChainedOps) {
    BigInt result = BI("1000") / BI("25") + BI("7") * BI("3") - BI("10");
    // 1000/25=40, 7*3=21, 40+21-10=51
    EXPECT_EQ(result.str(), "51");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
