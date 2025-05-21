#include "../include/AVL.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(AVLTest, InsertFindContains) {
	AVL<int,int> t;
	EXPECT_EQ(t.size(), 0u);
	t.insert(10,100);
	t.insert(20,200);
	t.insert(30,300);
	// после вставок сбалансировано (LL, RR, LR, RL)
	EXPECT_EQ(t.size(), 3u);
	EXPECT_TRUE(t.contains(20));
	int* p = t.find(30);
	ASSERT_NE(p, nullptr);
	EXPECT_EQ(*p,300);
}

TEST(AVLTest, RemoveLeafAndSingleChild) {
	AVL<int,int> t;
	for(int k: {3,2,1}) t.insert(k,k);
	EXPECT_EQ(t.size(),3u);
	t.remove(1);
	EXPECT_FALSE(t.contains(1));
	EXPECT_EQ(t.size(),2u);
	t.remove(2);
	EXPECT_FALSE(t.contains(2));
	EXPECT_EQ(t.size(),1u);
}

TEST(AVLTest, RemoveTwoChildren) {
	AVL<int,int> t;
	for(int k: {50,30,70,20,40,60,80}) t.insert(k,k);
	EXPECT_EQ(t.size(),7u);
	t.remove(50);
	EXPECT_FALSE(t.contains(50));
	// проверяем inorder-порядок
	std::vector<int> seq;
	t.inorder([&](const int& k,const int&){ seq.push_back(k); });
	EXPECT_EQ(seq, (std::vector<int>{20,30,40,60,70,80}));
}

TEST(AVLTest, IteratorInorder) {
	AVL<int,int> t;
	for(int k: {5,1,4,2,3}) t.insert(k,k);
	std::vector<int> seq;
	for(auto it=t.begin(); it!=t.end(); ++it)
		seq.push_back(it->first);
	EXPECT_EQ(seq, (std::vector<int>{1,2,3,4,5}));
}
