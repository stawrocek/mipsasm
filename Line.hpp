#ifndef LINE_HPP
#define LINE_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class Line{
public:
	enum LineType{
		R,
		I,
		J,
		Label,
		Directive,
		Unknown
	};
	Line();
	~Line();
	Line(std::string, std::vector<std::string>, unsigned int);
	friend std::ostream& operator<< (std::ostream& stream, const Line&);

	uint32_t address;
	uint32_t section;
	std::string line;
	std::vector<std::string> args;
	uint32_t lineIndex;
	uint32_t hexData;
	std::string longerData;
	LineType type;
};

#endif //LINE_HPP
