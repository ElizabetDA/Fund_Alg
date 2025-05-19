#include "MyMap.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(MyMapTest, BasicOperations) {
	MyMap<std::string,int> m;
	EXPECT_EQ(m.size(), 0u);

	m.insert("one", 1);
	m.insert("two", 2);
	EXPECT_EQ(m.size(), 2u);
	EXPECT_TRUE(m.contains("one"));
	EXPECT_FALSE(m.contains("three"));

	int* p = m.find("two");
	ASSERT_NE(p, nullptr);
	EXPECT_EQ(*p, 2);

	m.remove("one");
	EXPECT_FALSE(m.contains("one"));
	EXPECT_EQ(m.size(), 1u);

	m.clear();
	EXPECT_EQ(m.size(), 0u);
}

TEST(MyMapTest, IteratorAndTraversal) {
	MyMap<int,int> m;
	for (int i = 5; i >= 1; --i)
		m.insert(i, i*100);

	std::vector<int> keys;
	for (auto& kv : m)
		keys.push_back(kv.first);
	EXPECT_EQ(keys, std::vector<int>({1,2,3,4,5}));

	std::vector<int> out;
	m.inorder([&](const int& k, const int& v){ out.push_back(v); });
	EXPECT_EQ(out, std::vector<int>({100,200,300,400,500}));
}
