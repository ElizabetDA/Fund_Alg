#include "../include/Server.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: main_server <port>\n";
		return 1;
	}
	try {
		Server srv(static_cast<uint16_t>(std::stoi(argv[1])));
		srv.run();
	} catch (const std::exception& ex) {
		std::cerr << "Fatal: " << ex.what() << std::endl;
		return 1;
	}
	return 0;
}
