#include <iostream>
#include "../include/RedBlack.hpp"
#include "../include/MyMap.hpp"

int main() {
	MyMap<int,std::string,RedBlack> m;
	m.insert(5, "five");
	m.insert(1, "one");
	m.insert(9, "nine");
	m.insert(3, "three");
	m.insert(7, "seven");

	std::cout << "Red-Black tree inorder:\n";
	for (auto& kv : m) {
		std::cout << kv.first << " -> " << kv.second << "\n";
	}

	std::cout << "\nFind 3: ";
	if (auto* p = m.find(3)) std::cout << *p << "\n";

	std::cout << "\nRemove 5 and traverse:\n";
	m.remove(5);
	for (auto& kv : m) {
		std::cout << kv.first << " -> " << kv.second << "\n";
	}

	return 0;
}
