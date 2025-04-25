#include <gtest/gtest.h>
#include <string>
#include <memory>
#include "array.hpp"

using namespace my_container;

template <typename T, size_t N>
using MyArray = Array<T, N>;

class ArrayTest : public ::testing::Test {
protected:
	MyArray<int, 3> arr = {1, 2, 3};
	MyArray<int, 3> empty_arr;
};

TEST_F(ArrayTest, DefaultConstructor) {
	EXPECT_EQ(empty_arr.size(), 3);
	for (const auto& item : empty_arr) {
		EXPECT_EQ(item, 0);
	}
}

TEST_F(ArrayTest, InitializerList) {
	EXPECT_EQ(arr[0], 1);
	EXPECT_THROW((MyArray<int, 3>{1, 2}), std::invalid_argument);
}

TEST_F(ArrayTest, CopyConstructor) {
	MyArray<int, 3> copy(arr);
	EXPECT_EQ(arr, copy);
}

TEST_F(ArrayTest, MoveConstructor) {
	MyArray<std::string, 2> src{"Hello", "World"};
	MyArray<std::string, 2> dest(std::move(src));
	EXPECT_EQ(dest[0], "Hello");
}

TEST_F(ArrayTest, ElementAccess) {
	EXPECT_EQ(arr.at(0), 1);
	EXPECT_THROW(arr.at(3), std::out_of_range);
	arr[1] = 5;
	EXPECT_EQ(arr[1], 5);
}

TEST_F(ArrayTest, FrontBackData) {
	EXPECT_EQ(arr.front(), 1);
	EXPECT_EQ(arr.back(), 3);
	arr.data()[0] = 100;
	EXPECT_EQ(arr[0], 100);
}

TEST_F(ArrayTest, Iterators) {
	int sum = 0;
	for (const auto& item : arr) sum += item;
	EXPECT_EQ(sum, 6);
}

TEST_F(ArrayTest, ReverseIterators) {
	int sum = 0;
	for (auto it = arr.rbegin(); it != arr.rend(); ++it) sum += *it;
	EXPECT_EQ(sum, 6);
}

TEST_F(ArrayTest, FillAndSwap) {
	empty_arr.fill(42);
	for (const auto& item : empty_arr) EXPECT_EQ(item, 42);

	MyArray<int, 3> a = {1, 2, 3};
	MyArray<int, 3> b = {4, 5, 6};
	a.swap(b);
	EXPECT_EQ(a[0], 4);
	EXPECT_EQ(b[0], 1);
}

TEST_F(ArrayTest, Comparisons) {
	MyArray<int, 3> a = {1, 2, 3};
	MyArray<int, 3> b = {1, 2, 4};
	EXPECT_LT(a, b);
	EXPECT_NE(a, b);
	EXPECT_EQ(a, arr);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}