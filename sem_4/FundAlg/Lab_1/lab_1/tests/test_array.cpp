#include "../include/array.hpp"

#include <gtest/gtest.h>

namespace my_container {


class ArrayTest : public ::testing::Test {
   protected:
	using IntArray5 = Array<int, 5>;
	IntArray5 default_array;
	IntArray5 init_array{1, 2, 3, 4, 5};
	IntArray5 another_array{6, 7, 8, 9, 10};
	void SetUp() override {
	}
};

template <typename T, size_t N>
void test_array_content(const Array<T, N>& arr, const std::vector<T>& expected) {
	for (size_t i = 0; i < expected.size(); ++i) {
		EXPECT_EQ(arr[i], expected[i]);
	}
}

TEST_F(ArrayTest, DefaultConstructor) {
	test_array_content(default_array, {0, 0, 0, 0, 0});
}

TEST_F(ArrayTest, InitializerListConstructor) {
	test_array_content(init_array, {1, 2, 3, 4, 5});
	Array<int, 5> short_init{1, 2};
	test_array_content(short_init, {1, 2, 0, 0, 0});
}

TEST_F(ArrayTest, CopyConstructor) {
	IntArray5 copy(init_array);
	test_array_content(copy, {1, 2, 3, 4, 5});
}

TEST_F(ArrayTest, MoveConstructor) {
	IntArray5 moved(std::move(init_array));
	test_array_content(moved, {1, 2, 3, 4, 5});
}

TEST_F(ArrayTest, AssignmentOperator) {
	default_array = init_array;
	test_array_content(default_array, {1, 2, 3, 4, 5});
	default_array = default_array;  // self-assign
	test_array_content(default_array, {1, 2, 3, 4, 5});
}

TEST_F(ArrayTest, SubscriptOperator) {
	init_array[0] = 10;
	EXPECT_EQ(init_array[0], 10);
	const auto& const_ref = init_array;
	EXPECT_EQ(const_ref[1], 2);
}

TEST_F(ArrayTest, ComparisonOperators) {
	IntArray5 arr1{1, 2, 3, 4, 5};
	IntArray5 arr2{1, 2, 3, 4, 5};
	IntArray5 arr3{2, 1, 3, 4, 5};

	EXPECT_EQ(arr1, arr2);
	EXPECT_NE(arr1, arr3);
	EXPECT_LT(arr1, arr3);
	EXPECT_GT(arr3, arr1);
}

TEST_F(ArrayTest, SwapMethod) {
	init_array.swap(another_array);
	test_array_content(init_array, {6, 7, 8, 9, 10});
	test_array_content(another_array, {1, 2, 3, 4, 5});
}

TEST_F(ArrayTest, AtMethod) {
	EXPECT_EQ(init_array.at(0), 1);
	EXPECT_THROW(init_array.at(5), std::out_of_range);
	const auto& const_ref = init_array;
	EXPECT_EQ(const_ref.at(4), 5);
	EXPECT_THROW(const_ref.at(5), std::out_of_range);
}

TEST_F(ArrayTest, FrontBackMethods) {
	EXPECT_EQ(init_array.front(), 1);
	EXPECT_EQ(init_array.back(), 5);
	const auto& const_ref = init_array;
	EXPECT_EQ(const_ref.front(), 1);
	EXPECT_EQ(const_ref.back(), 5);
}

TEST_F(ArrayTest, DataMethod) {
	int* data = init_array.data();
	EXPECT_EQ(data[0], 1);
	const auto& const_ref = init_array;
	const int* cdata = const_ref.data();
	EXPECT_EQ(cdata[4], 5);
}

TEST_F(ArrayTest, Iterators) {
	size_t i = 0;
	for (auto it = init_array.begin(); it != init_array.end(); ++it, ++i) {
		EXPECT_EQ(*it, init_array[i]);
	}
	i = 0;
	for (auto it = init_array.cbegin(); it != init_array.cend(); ++it, ++i) {
		EXPECT_EQ(*it, init_array[i]);
	}
}

TEST_F(ArrayTest, ReverseIterators) {
	auto rit = init_array.rbegin();
	EXPECT_EQ(*rit, 5);
	++rit;
	EXPECT_EQ(*rit, 4);
	const auto& const_ref = init_array;
	auto crit = const_ref.crbegin();
	EXPECT_EQ(*crit, 5);
}

TEST_F(ArrayTest, SizeMethods) {
	EXPECT_EQ(init_array.size(), 5);
	EXPECT_EQ(init_array.max_size(), 5);
	EXPECT_FALSE(init_array.empty());
}

TEST_F(ArrayTest, FillMethod) {
	init_array.fill(42);
	test_array_content(init_array, {42, 42, 42, 42, 42});
}

TEST_F(ArrayTest, AtOutOfRange) {
	EXPECT_THROW(default_array.at(5), std::out_of_range);
	const auto& const_ref = default_array;
	EXPECT_THROW(const_ref.at(5), std::out_of_range);
}

}


int main(int argc, char * argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}