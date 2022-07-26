#include "atat/exceptions.hh"
#include <cstdio>

namespace atat
{

unimplemented_instruction_exception::unimplemented_instruction_exception(uint8_t op) throw() :
	std::runtime_error{"Unimplemented instruction"}
	{
		printf("Unimplemented instruction: {%x}\n", op);
	}

char const*
unimplemented_instruction_exception::what() const throw()
{
	// TODO display instruction opcode
	return "Unimplemented instruction";
}

} // namespace atat
