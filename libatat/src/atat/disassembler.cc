#include "atat/disassembler.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace atat
{

std::vector<byte_t>
memory_with_rom(std::string_view path, word_t offset)
{
	std::ifstream file{path.data(), std::ios::binary};
	std::cout << "Opening: \"" << path << "\"\n";
	if(!file.good()) {
		std::cerr << "Failed to open file\n";
		exit(1);
	}

	file.seekg(0, std::ios::end);
	std::size_t size = file.tellg();
	if((size + offset) > 0x4000) {
		throw std::runtime_error{"ROM too big"};
	}
	file.seekg(0, std::ios::beg);

	std::vector<byte_t> contents(0x4000);
	file.read(reinterpret_cast<char*>(contents.data() + offset), size);

	file.close();
	return contents;
}

} // namespace atat
