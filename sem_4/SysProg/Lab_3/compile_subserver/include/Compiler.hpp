#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>
#include <stdexcept>
#include <cstdlib>

class Compiler {
public:
	static void compile(const std::string& filename) {
		std::string cmd;
		if (filename.ends_with(".cpp")) {
			cmd = "g++ -std=c++20 -o " + filename + ".out " + filename;
		} else if (filename.ends_with(".tex")) {
			cmd = "pdflatex -interaction=nonstopmode " + filename;
		} else {
			throw std::runtime_error("Unsupported extension");
		}
		if (std::system(cmd.c_str()) != 0) {
			throw std::runtime_error("Compilation failed");
		}
	}
};

#endif // COMPILER_HPP
