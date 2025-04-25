#include <gtest/gtest.h>
#include "../include/deque.hpp"

using namespace my_container;

class DequeTest : public ::testing::Test {
protected:
    Deque<int> d;

    void SetUp() override {
        d.push_back(10);
        d.push_back(20);
        d.push_back(30);
    }
};

TEST_F(DequeTest, DefaultConstructor) {
    Deque<int> dq;
    EXPECT_TRUE(dq.empty());
    EXPECT_EQ(dq.size(), 0);
}

TEST_F(DequeTest, InitializerListConstructor) {
    Deque<int> dq{1, 2, 3};
    EXPECT_EQ(dq.size(), 3);
    EXPECT_EQ(dq.front(), 1);
    EXPECT_EQ(dq.back(), 3);
}

TEST_F(DequeTest, CopyConstructor) {
    Deque<int> copy(d);
    EXPECT_EQ(copy.size(), d.size());
    EXPECT_EQ(copy.front(), d.front());
    EXPECT_EQ(copy.back(), d.back());
}

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> moved(std::move(d));
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
    EXPECT_TRUE(d.empty());
}

TEST_F(DequeTest, AssignmentOperators) {
    Deque<int> dq;
    dq = d;
    EXPECT_EQ(dq.size(), d.size());
    EXPECT_EQ(dq.front(), d.front());

    Deque<int> dq2;
    dq2 = std::move(dq);
    EXPECT_EQ(dq2.front(), 10);
    EXPECT_TRUE(dq.empty());
}

TEST_F(DequeTest, AtMethod) {
    EXPECT_EQ(d.at(0), 10);
    EXPECT_EQ(d.at(1), 20);
    EXPECT_EQ(d.at(2), 30);
    EXPECT_THROW(d.at(3), std::out_of_range);
}

TEST_F(DequeTest, IndexOperator) {
    EXPECT_EQ(d[0], 10);
    EXPECT_EQ(d[1], 20);
    EXPECT_EQ(d[2], 30);
}

TEST_F(DequeTest, PushPopFrontBack) {
    d.push_front(5);
    EXPECT_EQ(d.front(), 5);

    d.pop_front();
    EXPECT_EQ(d.front(), 10);

    d.push_back(40);
    EXPECT_EQ(d.back(), 40);

    d.pop_back();
    EXPECT_EQ(d.back(), 30);
}

TEST_F(DequeTest, Resize) {
    d.resize(5, 99);
    EXPECT_EQ(d.size(), 5);
    EXPECT_EQ(d[4], 99);

    d.resize(2);
    EXPECT_EQ(d.size(), 2);
    EXPECT_EQ(d.back(), 20);
}

TEST_F(DequeTest, Swap) {
    Deque<int> other{100, 200};
    d.swap(other);
    EXPECT_EQ(d.front(), 100);
    EXPECT_EQ(other.front(), 10);
}

TEST_F(DequeTest, Comparisons) {
    Deque<int> equal{10, 20, 30};
    Deque<int> less{10, 20};
    Deque<int> greater{10, 20, 40};

    EXPECT_EQ(d, equal);
    EXPECT_NE(d, less);
    EXPECT_LT(d, greater);
    EXPECT_GT(greater, d);
    EXPECT_LE(less, d);
    EXPECT_GE(d, equal);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
