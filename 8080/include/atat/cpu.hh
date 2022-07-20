#pragma once

#include <cstdint>
#include <vector>

#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

namespace atat
{

struct condition_codes
{
	uint8_t z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	//uint8_t pad:3;

	[[nodiscard]]
	constexpr
	condition_codes() noexcept :
		z{0},
		s{0},
		p{0},
		cy{0},
		ac{0} {}
};

struct registers
{
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;

	[[nodiscard]]
	constexpr
	registers() noexcept :
		a{0},
		b{0},
		c{0},
		d{0},
		e{0},
		h{0},
		l{0} {}
};

class cpu
{
	// registers
	registers regs_;

	uint16_t sp_;
	uint16_t pc_;

	std::vector<uint8_t> memory_;

	condition_codes cc_;
	uint8_t int_enable_;

public:
	[[nodiscard]]
	cpu(std::vector<uint8_t>&& memory) noexcept :
		regs_{},
		sp_{},
		pc_{},
		memory_{memory},
		cc_{},
		int_enable_{} {}

	constexpr
	void
	step()
	{
		uint8_t op = memory_[pc_];

		switch(op)
		{
			default: {
				throw unimplemented_instruction_exception{op};
				break;
			}
			case opcodes::nop: { ++pc_; break; }
		}
	}
};

} // namespace atat
