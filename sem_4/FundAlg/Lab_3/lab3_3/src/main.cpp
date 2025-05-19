#include <iostream>
#include "../include/Splay.hpp"
#include "../include/MyMap.hpp"

int main() {
	MyMap<int, std::string, Splay> m;
	m.insert(50, "fifty");
	m.insert(20, "twenty");
	m.insert(80, "eighty");
	m.insert(10, "ten");
	m.insert(30, "thirty");

	std::cout << "Splay-дерево после нескольких вставок (inorder):\n";
	for (auto& kv : m) {
		std::cout << kv.first << " -> " << kv.second << "\n";
	}

	std::cout << "\nДоступ к элементу 30 (сплей-операция):\n";
	auto* p = m.find(30);
	if (p) std::cout << "Найдено: " << *p << "\n";

	std::cout << "\nУдаляем 50 и снова обход:\n";
	m.remove(50);
	for (auto& kv : m) {
		std::cout << kv.first << " -> " << kv.second << "\n";
	}

	return 0;
}
