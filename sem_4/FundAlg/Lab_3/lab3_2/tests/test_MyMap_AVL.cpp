#include "../include/MyMap.hpp"
#include "../include/AVL.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(MyMapAVLTest, BasicOperations) {
	MyMap<int,int,AVL> m;
	EXPECT_EQ(m.size(), 0u);
	m.insert(2,20);
	m.insert(1,10);
	m.insert(3,30);
	EXPECT_TRUE(m.contains(1));
	EXPECT_TRUE(m.contains(3));
	int* p = m.find(2);
	ASSERT_NE(p,nullptr);
	EXPECT_EQ(*p,20);
	m.remove(1);
	EXPECT_FALSE(m.contains(1));
	m.clear();
	EXPECT_EQ(m.size(),0u);
}

TEST(MyMapAVLTest, IteratorAndTraversal) {
	MyMap<int,int,AVL> m;
	for(int i=5;i>=1;--i) m.insert(i,i*10);
	std::vector<int> keys;
	for(auto& kv : m) keys.push_back(kv.first);
	EXPECT_EQ(keys, (std::vector<int>{1,2,3,4,5}));
	std::vector<int> vals;
	m.inorder([&](auto&, auto& v){ vals.push_back(v); });
	EXPECT_EQ(vals, (std::vector<int>{10,20,30,40,50}));
}
