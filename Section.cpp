#include "Section.hpp"

#include <iomanip>
#include <iostream>

Section::Section(std::string _name, uint32_t _currentAddr)
:name(_name), currentAddr(_currentAddr), alignment(0)
{

}

std::string toHex(const std::string& s, bool ascii=true){
	std::ostringstream ret;
	int minus = 0;
	if(!ascii)
		minus = '0';
	for(uint32_t i = 0; i < s.length(); ++i)
		ret << std::hex << std::setfill('0') << std::setw(2)
			<< std::nouppercase << (int)s[i]-minus;

	return ret.str();
}

int64_t string_to_number(std::string);

std::string numb_to_hexstring(uint32_t x, std::string pad){
	char tmp[100];
	sprintf(tmp, pad.c_str(), x);
	return std::string(tmp);
}

void Section::addInstruction(Line line){
	line.address = currentAddr;
	//«.align», «.ascii», «.asciiz», «.byte»,
	//«.data», «.half», «.space», «.text», «.word» i «.org»
	if(line.type == Line::Directive){
		if(line.args[0] == ".align"){
			alignment = string_to_number(line.args[1]);
			uint32_t p = 1;
			for(uint32_t i = 0; i < alignment; i++)
				p *= 2;
			if(currentAddr%p != 0){
				line.longerData.resize(2*(p-(currentAddr%p)), '0');
				currentAddr += (p - (currentAddr%p));
			}
			alignment = 0;
		}
		else if(line.args[0] == ".ascii" || line.args[0] == ".asciiz"){
			auto s = line.args[1].substr(1, line.args[1].size()-2); //""
			line.longerData = toHex(s);
			if(line.args[0] == ".asciiz"){
				line.longerData += "00";
				currentAddr++;
			}
			currentAddr += s.size();
		}
		else if(line.args[0] == ".byte" || line.args[0] == ".half"
						|| line.args[0] == ".word"){
			std::string res;
			std::string fmt = "%02x";
			if(line.args[0] == ".half")
				fmt = "%04x";
			if(line.args[0] == ".word")
				fmt = "%08x";
			for(uint32_t i = 1; i < line.args.size(); i++){
				res += numb_to_hexstring(string_to_number(line.args[i]),fmt);
			}
			currentAddr += res.size()/2;
			line.longerData = res;
		}
		else if(line.args[0] == ".space"){
			line.longerData.resize(string_to_number(line.args[1])*2, '0');
			currentAddr += line.longerData.size()/2;
		}
		else if(line.args[0] == ".org"){
			if(string_to_number(line.args[1]) < 0)
				throw std::invalid_argument(".org address can't be negative\n");
			currentAddr += string_to_number(line.args[1]);
		}
	}
	else if(line.type != Line::Label){
		currentAddr += 4;
	}
	/*if(alignment != 0){
		uint32_t p = 1;
		for(uint32_t i = 0; i < alignment; i++)
			p *= 2;
		if(currentAddr%p != 0)
			currentAddr += (p - (currentAddr%p));
		alignment = 0;
	}*/
	content.insert({currentAddr, line});
}
