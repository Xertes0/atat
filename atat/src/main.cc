#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include <atat/disassembler.hh>
#include <atat/cpu.hh>

int main(int argc, char const** argv)
{
	std::cout << "Hello world\n";

	auto memory = atat::disassemble(std::string_view{argv[1]});
	auto cpu = atat::cpu{std::move(memory)};

	cpu.step();

	//int i = 8;
	//while(i--) {
	//	std::cout << std::hex << static_cast<int>(contents[7-i]) << '\n';
	//}

	return 0;
}
