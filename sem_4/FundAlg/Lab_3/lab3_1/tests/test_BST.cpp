#include "BST.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(BSTTest, InsertFindContains) {
	BST<int,int> t;
	EXPECT_EQ(t.size(), 0u);

	t.insert(5,50);
	t.insert(3,30);
	t.insert(7,70);
	t.insert(5,55);
	EXPECT_EQ(t.size(), 3u);

	EXPECT_TRUE(t.contains(5));
	EXPECT_TRUE(t.contains(3));
	EXPECT_TRUE(t.contains(7));
	EXPECT_FALSE(t.contains(4));

	int* p = t.find(5);
	ASSERT_NE(p, nullptr);
	EXPECT_EQ(*p, 55);
	EXPECT_EQ(t.find(4), nullptr);
}

TEST(BSTTest, RemoveLeafAndSingleChild) {
	BST<int,int> t;
	t.insert(5,0);
	t.insert(3,0);
	t.insert(7,0);
	t.insert(2,0);
	t.insert(4,0);
	EXPECT_EQ(t.size(), 5u);

	t.remove(2);
	EXPECT_FALSE(t.contains(2));
	EXPECT_EQ(t.size(), 4u);

	t.remove(3);
	EXPECT_FALSE(t.contains(3));
	EXPECT_EQ(t.size(), 3u);
}

TEST(BSTTest, RemoveTwoChildren) {
	BST<int,int> t;
	for (int k : {5,3,7,2,4,6,8})
		t.insert(k, k*10);
	EXPECT_EQ(t.size(), 7u);

	t.remove(5);
	EXPECT_FALSE(t.contains(5));
	EXPECT_EQ(t.size(), 6u);
	EXPECT_TRUE(t.contains(6));

	std::vector<int> keys;
	t.inorder([&](const int& k, const int&){ keys.push_back(k); });
	std::vector<int> exp = {2,3,4,6,7,8};
	EXPECT_EQ(keys, exp);
}

TEST(BSTTest, IteratorInorder) {
	BST<int,int> t;
	for (int k : {5,1,4,2,3})
		t.insert(k, k);
	std::vector<int> seq;
	for (auto it = t.begin(); it != t.end(); ++it)
		seq.push_back(it->first);
	std::vector<int> exp = {1,2,3,4,5};
	EXPECT_EQ(seq, exp);
}
