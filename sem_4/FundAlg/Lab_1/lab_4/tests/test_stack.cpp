#include <gtest/gtest.h>
#include "../include/stack.hpp"

using namespace my_container;

class StackTest : public ::testing::Test {
protected:
    Stack<int> s;

    void SetUp() override {
        s.push(1);
        s.push(2);
        s.push(3);
    }
};

TEST_F(StackTest, DefaultConstructor) {
    Stack<int> st;
    EXPECT_TRUE(st.empty());
    EXPECT_EQ(st.size(), 0);
}

TEST_F(StackTest, InitializerListConstructor) {
    Stack<int> st{5, 6, 7};
    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.top(), 7);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copy(s);
    EXPECT_EQ(copy.size(), s.size());
    EXPECT_EQ(copy.top(), s.top());
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> moved(std::move(s));
    EXPECT_EQ(moved.top(), 3);
    EXPECT_TRUE(s.empty());
}

TEST_F(StackTest, AssignmentOperators) {
    Stack<int> a;
    a = s;
    EXPECT_EQ(a.top(), s.top());
    EXPECT_EQ(a.size(), s.size());

    Stack<int> b;
    b = std::move(a);
    EXPECT_EQ(b.top(), 3);
    EXPECT_TRUE(a.empty());
}

TEST_F(StackTest, TopAccess) {
    EXPECT_EQ(s.top(), 3);
    s.pop();
    EXPECT_EQ(s.top(), 2);
}

TEST_F(StackTest, PushPop) {
    s.push(4);
    EXPECT_EQ(s.top(), 4);
    s.pop();
    EXPECT_EQ(s.top(), 3);
}

TEST_F(StackTest, PopFromEmptyThrows) {
    Stack<int> empty_stack;
    EXPECT_THROW(empty_stack.pop(), std::out_of_range);
}

TEST_F(StackTest, TopFromEmptyThrows) {
    Stack<int> empty_stack;
    EXPECT_THROW(empty_stack.top(), std::out_of_range);
}

TEST_F(StackTest, SwapTest) {
    Stack<int> other;
    other.push(100);
    s.swap(other);
    EXPECT_EQ(s.top(), 100);
    EXPECT_EQ(other.top(), 3);
}

TEST_F(StackTest, ComparisonOperators) {
    Stack<int> same{1, 2, 3};
    Stack<int> less{1, 2};
    Stack<int> more{1, 2, 4};

    EXPECT_EQ(s, same);
    EXPECT_NE(s, less);
    EXPECT_LT(s, more);
    EXPECT_GT(more, s);
    EXPECT_LE(less, s);
    EXPECT_GE(s, same);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
