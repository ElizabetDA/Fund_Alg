#include <gtest/gtest.h>
#include "../include/big_int.hpp"
#include <sstream>

using lab2::BigInt;

TEST(BigIntTest, DefaultConstruction) {
	EXPECT_EQ(BigInt(), BigInt(0));
}

TEST(BigIntTest, FromLongLong) {
	BigInt x(1234567890LL);
	std::stringstream ss; ss << x;
	EXPECT_EQ(ss.str(), "1234567890");
}

TEST(BigIntTest, FromStringAndStreamIO) {
	BigInt x("-987654321");
	std::stringstream ss; ss << x;
	EXPECT_EQ(ss.str(), "-987654321");
	BigInt y; ss >> y;
	EXPECT_EQ(x, y);
}

TEST(BigIntTest, Addition) {
	EXPECT_EQ(BigInt("123") + BigInt("877"), BigInt("1000"));
	EXPECT_EQ(BigInt("-50") + BigInt("20"), BigInt("-30"));
}

TEST(BigIntTest, Subtraction) {
	EXPECT_EQ(BigInt("1000") - BigInt("1"), BigInt("999"));
	EXPECT_EQ(BigInt("1") - BigInt("1000"), BigInt("-999"));
}

TEST(BigIntTest, Multiplication) {
	EXPECT_EQ(BigInt("123") * BigInt("0"), BigInt("0"));
	EXPECT_EQ(BigInt("-25") * BigInt("4"), BigInt("-100"));
}

TEST(BigIntTest, Division) {
	EXPECT_EQ(BigInt("100") / BigInt("20"), BigInt("5"));
	EXPECT_EQ(BigInt("-100") / BigInt("20"), BigInt("-5"));
}

TEST(BigIntTest, Comparison) {
	EXPECT_TRUE(BigInt("123") > BigInt("-456"));
	EXPECT_TRUE(BigInt("999") < BigInt("1000"));
	EXPECT_TRUE(BigInt("0") == BigInt());
}
