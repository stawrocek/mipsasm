#include "Line.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

Line::Line(){}
Line::~Line(){}

Line::Line(std::string _line, std::vector<std::string> _args,
	unsigned int _lineIndex)
:address(0), section(0), line(_line), args(_args),
	lineIndex(_lineIndex), hexData(0), type(Unknown)
{
	longerData="";
}

std::ostream& operator<< (std::ostream& stream, const Line& line){
	if(line.type == Line::Label){
		std::cout << line.args[0] << "\n";
	}
	else if(line.type == Line::Directive && line.longerData.empty()){
		std::cout << line.line << "\n";
	}
	else{
		std::cout << std::setfill('0') << std::setw(8) << std::hex
			<< line.address << "\t";
		if(line.longerData.empty())
			std::cout << std::setfill('0') << std::setw(8) << std::hex
				<< line.hexData << "\t\t";
		else{
			std::cout << line.longerData << "\t\t";
		}
		std::cout << line.line << "\n";
	}
	return stream;
}
