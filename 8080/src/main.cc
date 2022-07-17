#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include "atat/disassembler.hh"

int main(int argc, char const** argv)
{
	std::cout << "Hello world\n";

	auto contents = atat::disassemble(std::string_view{argv[1]});

	int i = 8;
	while(i--) {
		std::cout << std::hex << static_cast<int>(contents[7-i]) << '\n';
	}

	return 0;
}
