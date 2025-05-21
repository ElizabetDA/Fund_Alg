#include "MyMap.hpp"
#include "RedBlack.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(MyMapRBTest, BasicOperations) {
	MyMap<std::string,int,RedBlack> m;
	EXPECT_EQ(m.size(),0u);
	m.insert("one",1);
	m.insert("two",2);
	EXPECT_TRUE(m.contains("one"));
	EXPECT_FALSE(m.contains("three"));
	int* p = m.find("two");
	ASSERT_NE(p,nullptr);
	EXPECT_EQ(*p,2);
	m.remove("one");
	EXPECT_FALSE(m.contains("one"));
	m.clear();
	EXPECT_EQ(m.size(),0u);
}

TEST(MyMapRBTest, IteratorAndTraversal) {
	MyMap<int,int,RedBlack> m;
	for(int i=5;i>=1;--i) m.insert(i,i*10);
	std::vector<int> keys;
	for(auto& kv : m) keys.push_back(kv.first);
	EXPECT_EQ(keys, (std::vector<int>{1,2,3,4,5}));
	std::vector<int> vals;
	m.inorder([&](auto&, auto& v){ vals.push_back(v); });
	EXPECT_EQ(vals, (std::vector<int>{10,20,30,40,50}));
}
