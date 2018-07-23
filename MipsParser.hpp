#ifndef MIPS_PARSER_HPP
#define MIPS_PARSER_HPP

#include <cstdint>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "Line.hpp"
#include "Section.hpp"

class MipsParser{
public:
	MipsParser(std::string);
	void assembly(Line&);
	void assemblyI(Line&);
	void assemblyJ(Line&);
	void assemblyR(Line&);
	void generateArchitectureMaps();
	std::vector<std::string> split(std::string&, uint32_t);

	std::vector<Line> program;
	std::map<std::string, uint32_t> regs;
	std::map<std::string, std::tuple<uint8_t,
			uint16_t, std::vector<std::string> > > instructions;
	std::vector<Section> sections;
	uint32_t sectionIdx;
};

#endif //MIPS_PARSER_HPP
