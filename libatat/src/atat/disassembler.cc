#include "atat/disassembler.hh"

#include <fstream>
#include <iostream>

namespace atat
{

std::vector<uint8_t>
disassemble(std::string_view path)
{
	std::ifstream file{path.data(), std::ios::binary};
	std::cout << "Opening: \"" << path << "\"\n";
	if(!file.good()) {
		std::cerr << "Failed to open file\n";
		exit(1);
	}

	file.seekg(0, std::ios::end);
	std::size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> contents(size);
	file.read(reinterpret_cast<char*>(contents.data()), size);

	file.close();
	return contents;
}

} // namespace atat
