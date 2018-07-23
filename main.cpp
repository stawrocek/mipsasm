#include <iostream>
#include <fstream>
#include <streambuf>

#include "MipsParser.hpp"

int main(int argc, char* argv[]){
	if(argc != 2){
		std::cout << "Usage: ./mipsasm filename.asm\n";
		return -1;
	}
	std::ifstream f(argv[1]);
	std::string str((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	MipsParser p(str);
}
