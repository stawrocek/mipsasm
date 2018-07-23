#include "MipsParser.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <tuple>

MipsParser::MipsParser(std::string code){
	generateArchitectureMaps();
	sections = {Section(".text", 0x00400000),
							Section(".data", 0x10010000)};
	sectionIdx=0;
	std::vector<std::string> lines;	
	std::istringstream stream(code);
	std::string tmp;
	while(std::getline(stream, tmp)) {
		lines.push_back(tmp);
	}
	std::vector<std::vector<std::string> > vec;	
	for(uint32_t i = 0; i < lines.size(); i++){
		const auto& spl = split(lines[i], i);
		if(!spl.empty()){
			program.push_back(Line(lines[i], spl, i));
		}
	}
	for(uint32_t i = 0; i < program.size(); i++){
		assembly(program[i]);
		//std::cout << program[i];
		sections[sectionIdx].addInstruction(program[i]);
	}
	for(const auto& s: sections){
		for(const auto& line: s.content)
			std::cout << line.second;
		std::cout << "\n";
	}
}

void MipsParser::assembly(Line& line){
	const auto& instr = instructions.find(line.args[0]);
	if(instr != instructions.end()){
		if(std::get<0>(instr->second) == 'R')
			line.type = Line::R;
		if(std::get<0>(instr->second) == 'I')
			line.type = Line::I;
		else if (std::get<0>(instr->second) == 'J')
			line.type = Line::J;
	}
	else{
		if(line.args[0][0] == '.')
			line.type = Line::Directive;
		else if(line.args[0].back() == ':')
			line.type = Line::Label;
	}
	if(line.type == Line::R){
		assemblyR(line);
	}
	else if(line.type == Line::I){
		assemblyI(line);
	}
	else if(line.type == Line::J){

	}
	else if(line.type == Line::Directive){
		for(uint32_t i = 0; i < sections.size(); i++){
			if(sections[i].name == line.args[0]){
				sectionIdx=i;
			}
		}
	}
	else if(line.type == Line::Label){
		
	}
	else{
		if(line.args[0] == "syscall")
			line.hexData = 0xc;
	}
}

int64_t string_to_number(std::string str){
	if(str.size() > 2){
		if(str[0] == '0' && str[1] == 'x')
			return std::stol(str, 0, 16);
		if(str[0] == '0' && str[1] == 'b')
			return std::stol(str, 0, 2);
	}
	return std::stol(str);
}

void MipsParser::assemblyI(Line& line){
	line.hexData = 0;
	const auto& instr = instructions.find(line.args[0]);
	const auto& encoding = std::get<2>(instr->second);
	std::string name = instr->first;
	uint16_t rs=0, rt=0, imm=0;
	uint16_t opcode = std::get<1>(instr->second);
	for(uint32_t i = 0; i < encoding.size(); i++){
		if(encoding[i] == "rs")
			rs = regs[line.args[i+1]];
		if(encoding[i] == "rt")
			rt = regs[line.args[i+1]];
		if(encoding[i] == "imm"){
			imm = string_to_number(line.args[i+1]);
		}
	}
	line.hexData = imm; //imm -> uint16_t
	line.hexData |= (rt << 16);
	line.hexData |= (rs << 21);
	line.hexData |= (opcode << 26);
}

void MipsParser::assemblyJ(Line& line){

}

void MipsParser::assemblyR(Line& line){
	line.hexData = 0;
	const auto& instr = instructions.find(line.args[0]);
	const auto& encoding = std::get<2>(instr->second);
	std::string name = instr->first;
	uint16_t rs=0, rt=0, rd=0, shamt=0;	
	uint16_t func = std::get<1>(instr->second);
	for(uint32_t i = 0; i < encoding.size(); i++){
		if(encoding[i] == "rs")
			rs = regs[line.args[i+1]];
		if(encoding[i] == "rt")
			rt = regs[line.args[i+1]];
		if(encoding[i] == "rd")
			rd = regs[line.args[i+1]];
		if(encoding[i] == "shamt")
			shamt = string_to_number(line.args[i+1]);
	}

	line.hexData |= func;
	line.hexData |= (shamt << 6);
	line.hexData |= (rd << 11);
	line.hexData |= (rt << 16);
	line.hexData |= (rs << 21);
}

void MipsParser::generateArchitectureMaps(){
	std::vector<std::tuple<std::string, int, int, int> > r = {
		{"$zero",	0,  1, -1},
		{"$at",		1,  1, -1},
		{"$v",		2,  2,  0},
		{"$a", 		4,  4,  0},
		{"$t",		8,  8,  0},
		{"$s",		16, 8,  0},
		{"$t",		24, 2,  8},
		{"$k",		26, 2,  0},
		{"$gp",		28, 1, -1},
		{"$sp",		29, 1, -1},
		{"$fp",		30, 1, -1},
		{"$ra",		31, 1, -1}
	};
	uint32_t ctr=0;
	for(uint32_t i = 0; i < r.size(); i++){
		std::string name = std::get<0>(r[i]);
		if(std::get<3>(r[i]) == -1)
			regs[name] = ctr++;
		else{
			for(int j = 0; j < std::get<2>(r[i]); j++){
				regs[name+std::to_string(std::get<3>(r[i])+j)] = ctr++;
			}
		}
	}
	for(uint32_t i = 0; i < 32; i++){
		regs["$"+std::to_string(i)] = i;
	}
	/*
		«lui», «addi», «addiu», «slti», «sltiu», «andi», «ori»,
«xori», «lui», «sll», «srl», «sra», «sllv», «srlv», «srav», «mfhi», «mthi», «mflo», «mtlo», «mult»,
«multu», «div», «divu», «add», «addu», «sub», «subu», «and», «or», «xor», «nor», «slt», «sltu».
	*/
	std::vector<std::string> timm {"rt", "imm"};
	std::vector<std::string> tsimm {"rt","rs","imm"};
	std::vector<std::string> dst {"rd", "rs", "rt"};
	std::vector<std::string> dts {"rd", "rt", "rs"};
	std::vector<std::string> st {"rs", "rt"};
	std::vector<std::string> dtshamt {"rd", "rt", "shamt"};
	instructions = {
		//{type, opcode[I,J]/funct[R]}
		{"lui", 	{'I',15,  timm}},
		{"addi", 	{'I',8,   tsimm}},
		{"addiu", {'I',9,   tsimm}},
		{"slti", 	{'I',10,  tsimm}},
		{"sltiu", {'I',11,  tsimm}},
		{"andi", 	{'I',12,  tsimm}},
		{"ori", 	{'I',13,  tsimm}},
		{"xori", 	{'I',14,  tsimm}},
		//{"lui", 	{'I',15,  {}}},   //hmmm...
		{"sll", 	{'R',0,   dtshamt}},
		{"srl", 	{'R',2,   dtshamt}},
		{"sra", 	{'R',3,   dtshamt}},
		{"sllv", 	{'R',4,   dts}},
		{"srlv", 	{'R',6,   dts}},
		{"srav", 	{'R',7,   dts}},
		{"mfhi", 	{'R',16,  {"rd"}}},
		{"mthi", 	{'R',17,  {"rs"}}},
		{"mflo", 	{'R',18,  {"rd"}}},
		{"mtlo", 	{'R',19,  {"rs"}}},
		{"mult", 	{'R',24,  st}},
		{"multu", {'R',25,  st}},
		{"div", 	{'R',26,  st}},
		{"divu", 	{'R',27,  st}},
		{"add", 	{'R',32,  dst}},
		{"addu", 	{'R',33,  dst}},
		{"sub", 	{'R',34,  dst}},
		{"subu", 	{'R',35,  dst}},
		{"and", 	{'R',36,  dst}},
		{"or", 		{'R',37,  dst}},
		{"xor", 	{'R',38,  dst}},
		{"nor", 	{'R',39,  dst}},
		{"slt", 	{'R',42,  dst}},
		{"sltu", 	{'R',43,  dst}}
	};
}

std::vector<std::string> MipsParser::split(std::string& line, uint32_t idx){
	std::vector<std::string> res;
	bool apo=false;
	std::string tmp;
	for(uint32_t i = 0; i < line.size(); i++){
		char c = line[i];
		if(c == '\"'){
			if(!tmp.empty()){
				if(apo)
					tmp="\""+tmp+"\"";
				res.push_back(tmp);
				tmp="";
			}
			apo = !apo;
		}
		else if(c == '#' && !apo){
			line = line.substr(0, i-1);
			break;
		}
		else if((c == ' ' || c == '\t' || c == ':' || c == ',') && !apo){
			if(!tmp.empty()){
				if(c == ':')
					tmp=tmp+":";
				res.push_back(tmp);
				tmp = "";
			}
		}
		else{
			tmp += c;
		}
	}
	if(apo)
		throw std::invalid_argument(std::to_string(idx) + ": wrong apostrophes\n");
	if(!tmp.empty())
		res.push_back(tmp);
	line.erase(line.find_last_not_of(" \t\n") + 1);
	return res;
}

