#ifndef SECTION_HPP
#define SECTION_HPP

#include "Line.hpp"

#include <cstdint>
#include <map>
#include <string>

class Section{
public:
	Section(std::string, uint32_t);
	std::string name;
	uint32_t currentAddr;
	uint32_t alignment=0;
	std::multimap<uint32_t, Line> content;
	void addInstruction(Line);
};

#endif //SECTION_HPP
