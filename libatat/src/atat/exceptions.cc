#include "atat/exceptions.hh"
#include <cstdio>

namespace atat
{

unimplemented_instruction_exception::unimplemented_instruction_exception() throw() :
	std::runtime_error{"Unimplemented instruction"} {}

unimplemented_instruction_exception::unimplemented_instruction_exception(uint8_t op) throw() :
	std::runtime_error{"Unimplemented instruction"}
	{
		fprintf(stderr, "Unimplemented instruction: {%x}\n", op);
	}

rom_write_exception::rom_write_exception() throw() :
	std::runtime_error("Tried to access read-only rom memory") {}

} // namespace atat
