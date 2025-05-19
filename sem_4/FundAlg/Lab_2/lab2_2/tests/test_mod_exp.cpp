#include <gtest/gtest.h>
#include "big_int.hpp"

using lab2::BigInt;
using lab2::mod_exp;

TEST(ModExp, Basic) {
	EXPECT_EQ(mod_exp(BigInt("2"),  BigInt("10"), BigInt("1000")), BigInt("24"));
	EXPECT_EQ(mod_exp(BigInt("3"),  BigInt("5"),  BigInt("7")),    BigInt("5"));
}

TEST(ModExp, EdgeCases) {
	EXPECT_EQ(mod_exp(BigInt("0"),   BigInt("123"), BigInt("97")), BigInt("0"));
	EXPECT_EQ(mod_exp(BigInt("999"), BigInt("0"),   BigInt("13")), BigInt("1"));
	EXPECT_EQ(mod_exp(BigInt("123"), BigInt("456"), BigInt("1")),  BigInt("0"));
}

TEST(ModExp, Large) {
	EXPECT_EQ(
		mod_exp(BigInt("2"), BigInt("1000"), BigInt("1000000007")),
		BigInt("688423210")
	);
}

TEST(ModExp, ModZeroThrows) {
	EXPECT_THROW(mod_exp(BigInt("2"), BigInt("5"), BigInt("0")), std::domain_error);
}
