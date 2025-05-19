#include <iostream>
#include "../include/BST.hpp"
#include "../include/MyMap.hpp"

int main() {
	std::cout << "BST & MyMap demo\n";
	MyMap<int,std::string> m;
	m.insert(10, "ten");
	m.insert(5, "five");
	m.insert(15, "fifteen");
	for (auto& kv : m) {
		std::cout << kv.first << " -> " << kv.second << "\n";
	}
	return 0;
}