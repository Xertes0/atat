#pragma once

#include <stdexcept>

namespace atat
{

class unimplemented_instruction_exception : public std::runtime_error
{
public:
	unimplemented_instruction_exception() throw();
	unimplemented_instruction_exception(uint8_t op) throw();
};

class rom_write_exception : public std::runtime_error
{
public:
	rom_write_exception() throw();
};

} // namespace atat
