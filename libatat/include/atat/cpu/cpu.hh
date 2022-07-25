#pragma once

/**
 * @file cpu.hh
 * 
 * Main cpu logic.
 */

#include <cstdint>
#include <vector>

#include "atat/cpu/flags.hh"
#include "atat/cpu/registers.hh"
#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

namespace atat
{

/**
 * CPU
 */
class cpu
{
private:
	registers regs_; ///< Registers

	uint16_t sp_; ///< Stact pointer
	uint16_t pc_; ///< Program counter

	uint8_t* memory_; ///< Cpu memory

	flags flags_; ///< Cpu flags
	uint8_t int_enable_; ///< Enable interrupts

public:
	/**
	 * Constructor
	 * 
	 * @param memory Pointer to cpu memory
	 */
	cpu(uint8_t* memory);

	/**
	 * Step cpu by a single instruction.
	 */
	void
	step();
};

} // namespace atat
