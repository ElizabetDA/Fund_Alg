#include <gtest/gtest.h>
#include "../include/vector.hpp"

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    Vector<int> v;

    void SetUp() override {
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
    }
};

TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> vec{10, 20, 30};
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.front(), 10);
    EXPECT_EQ(vec.back(), 30);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> copy(v);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.back(), 3);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> moved(std::move(v));
    EXPECT_EQ(moved.back(), 3);
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, AssignmentOperators) {
    Vector<int> a;
    a = v;
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], 1);

    Vector<int> b;
    b = std::move(a);
    EXPECT_EQ(b[2], 3);
    EXPECT_TRUE(a.empty());
}

TEST_F(VectorTest, AccessOperators) {
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    EXPECT_NO_THROW(v.at(2));
    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST_F(VectorTest, CapacityOperations) {
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

TEST_F(VectorTest, Modifiers) {
    v.push_back(4);
    EXPECT_EQ(v.back(), 4);
    v.pop_back();
    EXPECT_EQ(v.back(), 3);

    v.insert(1, 100);
    EXPECT_EQ(v[1], 100);

    v.erase(1);
    EXPECT_EQ(v[1], 2);

    v.resize(5, 42);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[4], 42);

    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> same{1, 2, 3};
    Vector<int> diff{1, 2};
    Vector<int> greater{1, 2, 4};

    EXPECT_EQ(v, same);
    EXPECT_NE(v, diff);
    EXPECT_LT(diff, v);
    EXPECT_GT(greater, v);
    EXPECT_LE(diff, v);
    EXPECT_GE(same, v);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}