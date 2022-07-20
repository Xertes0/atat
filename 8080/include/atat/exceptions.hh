#pragma once

#include <stdexcept>

namespace atat
{

class unimplemented_instruction_exception : public std::runtime_error
{
public:
	[[nodiscard]]
	unimplemented_instruction_exception(uint8_t op) throw();

	virtual char const* what() const throw();
};

} // namespace atat
