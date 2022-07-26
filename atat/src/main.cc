#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include <atat/disassembler.hh>
#include <atat/cpu/cpu.hh>

int main(int argc, char const** argv)
{
	std::cout << "Hello world\n";

	auto memory = atat::memory_with_rom(std::string_view{argv[1]});
	auto cpu = atat::cpu{memory.data()};

	std::cout << "Running\n";
	while (true) {
		cpu.step();
	}

	return 0;
}
