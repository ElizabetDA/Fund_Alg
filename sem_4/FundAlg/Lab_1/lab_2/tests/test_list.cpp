#include <gtest/gtest.h>
#include "../include/list.hpp"
#include <string>

using namespace my_container;

class ListTest : public ::testing::Test {
protected:
    List<int> list;
    void SetUp() override {
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
    }
};

TEST_F(ListTest, DefaultConstructor) {
    List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    List<int> l{10, 20, 30};
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 30);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> copy(list);
    EXPECT_EQ(copy, list);
}

TEST_F(ListTest, MoveConstructor) {
    List<int> moved(std::move(list));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, AssignmentOperators) {
    List<int> a;
    a = list;
    EXPECT_EQ(a, list);

    List<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 3);
    EXPECT_TRUE(a.empty());
}

TEST_F(ListTest, AccessFrontBack) {
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(ListTest, PushPopFrontBack) {
    list.push_front(0);
    EXPECT_EQ(list.front(), 0);

    list.pop_front();
    EXPECT_EQ(list.front(), 1);

    list.push_back(4);
    EXPECT_EQ(list.back(), 4);

    list.pop_back();
    EXPECT_EQ(list.back(), 3);
}

TEST_F(ListTest, Resize) {
    list.resize(5, 9);
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(list.back(), 9);

    list.resize(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);
}

TEST_F(ListTest, Swap) {
    List<int> other{9, 8};
    list.swap(other);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(other.size(), 3);
    EXPECT_EQ(list.front(), 9);
    EXPECT_EQ(other.front(), 1);
}

TEST_F(ListTest, InsertErase) {
    auto it = list.begin();
    ++it;
    list.insert(it, 99);
    it = list.begin();
    ++it;
    EXPECT_EQ(*it, 99);

    list.erase(it);
    it = list.begin();
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST_F(ListTest, Iteration) {
    int sum = 0;
    for (int val : list) {
        sum += val;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ListTest, ConstIteration) {
    const List<int>& constList = list;
    int sum = 0;
    for (auto it = constList.cbegin(); it != constList.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ListTest, Comparisons) {
    List<int> equal{1, 2, 3};
    List<int> smaller{1, 2};
    List<int> larger{1, 2, 4};

    EXPECT_TRUE(list == equal);
    EXPECT_TRUE(list != smaller);
    EXPECT_TRUE(list < larger);
    EXPECT_TRUE(larger > list);
    EXPECT_TRUE(smaller <= list);
    EXPECT_TRUE(list >= equal);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}