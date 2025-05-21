#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

#include "../include/BST.hpp"
#include "../include/AVL.hpp"
#include "../include/Splay.hpp"
#include "../include/RedBlack.hpp"
#include "../include/MyMap.hpp"


using Clock = std::chrono::high_resolution_clock;
using ms    = std::chrono::duration<double, std::milli>;

template<template<typename,typename,typename> class Tree>
void bench(const std::vector<int>& data,
		   double& t_insert, double& t_find, double& t_erase)
{
	MyMap<int,int,Tree> m;

	// вставка
	{
		auto t0 = Clock::now();
		for (auto x : data) m.insert(x, x);
		t_insert = ms(Clock::now() - t0).count();
	}
	// поиск
	{
		auto t0 = Clock::now();
		for (auto x : data) volatile auto p = m.find(x);
		t_find = ms(Clock::now() - t0).count();
	}
	// удаление
	{
		auto t0 = Clock::now();
		for (auto x : data) m.remove(x);
		t_erase = ms(Clock::now() - t0).count();
	}
}

int main(){
	std::vector<size_t> sizes = {1000, 100000, 1000000};
	std::mt19937_64 gen(123456);
	std::ofstream out("results.csv");
	out << "Tree,N,Insert_ms,Find_ms,Erase_ms\n";

	for (auto N : sizes) {
		std::vector<int> data(N);
		for (size_t i = 0; i < N; ++i) data[i] = int(gen());
		std::shuffle(data.begin(), data.end(), gen);

		for (auto name : {"BST","AVL","Splay","RedBlack"}) {
			double ti=0, tf=0, te=0;
			if      (std::string(name)=="BST")      bench<BST>(data, ti, tf, te);
			else if (std::string(name)=="AVL")      bench<AVL>(data, ti, tf, te);
			else if (std::string(name)=="Splay")    bench<Splay>(data, ti, tf, te);      // <-- добавили
			else if (std::string(name)=="RedBlack") bench<RedBlack>(data, ti, tf, te);

			out << name << ',' << N << ',' << ti << ',' << tf << ',' << te << '\n';
			std::cout
				<< name << " N=" << N
				<< " insert=" << ti  << "ms"
				<< " find="   << tf  << "ms"
				<< " erase="  << te  << "ms\n";
		}
		std::cout << "----------------------------------------\n";
	}
	return 0;
}

//g++ -O2 -std=c++17 src/benchmark.cpp -Iinclude -o bench
