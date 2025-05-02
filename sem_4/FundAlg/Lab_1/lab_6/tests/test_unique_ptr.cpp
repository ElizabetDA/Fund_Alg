#include <gtest/gtest.h>
#include "../include/unique_ptr.hpp"

using namespace my_smart_ptr;

class UniquePtrTest : public ::testing::Test {
protected:
	UniquePtr<int> uptr;
};

TEST_F(UniquePtrTest, DefaultConstructor) {
	EXPECT_FALSE(uptr);
	EXPECT_EQ(uptr.get(), nullptr);
}

TEST_F(UniquePtrTest, PointerConstructor) {
	UniquePtr<int> p(new int(42));
	EXPECT_TRUE(p);
	EXPECT_EQ(*p, 42);
}

TEST_F(UniquePtrTest, MoveConstructor) {
	UniquePtr<int> p1(new int(10));
	UniquePtr<int> p2(std::move(p1));
	EXPECT_TRUE(p2);
	EXPECT_EQ(*p2, 10);
	EXPECT_FALSE(p1);
}

TEST_F(UniquePtrTest, MoveAssignment) {
	UniquePtr<int> p1(new int(15));
	UniquePtr<int> p2;
	p2 = std::move(p1);
	EXPECT_TRUE(p2);
	EXPECT_EQ(*p2, 15);
	EXPECT_FALSE(p1);
}

TEST_F(UniquePtrTest, ResetAndRelease) {
	uptr.reset(new int(99));
	EXPECT_TRUE(uptr);
	EXPECT_EQ(*uptr, 99);

	int* raw = uptr.release();
	EXPECT_EQ(uptr.get(), nullptr);
	EXPECT_FALSE(uptr);
	delete raw;
}

TEST_F(UniquePtrTest, Swap) {
	UniquePtr<int> a(new int(1));
	UniquePtr<int> b(new int(2));
	a.swap(b);
	EXPECT_EQ(*a, 2);
	EXPECT_EQ(*b, 1);
}

// Тесты для UniquePtr<T[]> (массивов)
TEST(UniquePtrArrayTest, ArraySupport) {
	UniquePtr<int[]> arr(new int[3]{10, 20, 30});
	EXPECT_EQ(arr[0], 10);
	EXPECT_EQ(arr[1], 20);
	EXPECT_EQ(arr[2], 30);

	arr[1] = 99;
	EXPECT_EQ(arr[1], 99);
}

TEST(UniquePtrArrayTest, ReleaseAndResetArray) {
	UniquePtr<int[]> arr(new int[2]{1, 2});
	int* raw = arr.release();
	EXPECT_EQ(arr.get(), nullptr);
	EXPECT_FALSE(arr);
	delete[] raw;

	arr.reset(new int[2]{3, 4});
	EXPECT_EQ(arr[0], 3);
	EXPECT_EQ(arr[1], 4);
}
