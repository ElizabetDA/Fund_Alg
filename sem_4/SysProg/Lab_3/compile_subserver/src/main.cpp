#include "../include/SharedMemSem.hpp"
#include "../include/Compiler.hpp"
#include <iostream>

int main() {
	SharedMemSem shm(ftok(".", 'S'), sizeof(CompileTask));
	while (true) {
		auto task = shm.read();
		try {
			Compiler::compile(task.filename);
		} catch (const std::exception& ex) {
			std::cerr << "Compile error: " << ex.what() << std::endl;
		}
		// сообщаем серверу, что готово
		shm.write(task);
	}
	return 0;
}
