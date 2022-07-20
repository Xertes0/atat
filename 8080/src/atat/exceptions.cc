#include "atat/exceptions.hh"

namespace atat
{

[[nodiscard]]
unimplemented_instruction_exception::unimplemented_instruction_exception(uint8_t op) throw() :
	std::runtime_error{"Unimplemented instruction"} {}

char const*
unimplemented_instruction_exception::what() const throw()
{
	// TODO display instruction opcode
	return "Unimplemented instruction";
}

} // namespace atat
