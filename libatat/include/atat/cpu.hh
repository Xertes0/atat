#pragma once

/**
 * @file cpu.hh
 * 
 * Main cpu logic.
 */

#include <cstdint>
#include <vector>

#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

namespace atat
{

/**
 * Contains cpu flags.
 */
struct flags
{
	/**
	 * Zero bit.
	 * Set to 1 if result is zero.
	 */
	uint8_t z:1;
	
	/**
	 * Sign bit.
	 * Set to 1 if bit 7 is set.
	 */
	uint8_t s:1;

	/**
	 * Parity bit.
	 * Set to 1 if result has even parity.
	 */
	uint8_t p:1;

	/**
	 * Carry bit.
	 * Set to 1 if instruction resulted in carry out or borrow into higer bit.
	 */
	uint8_t c:1;

	//uint8_t a:1; // auxillary carry
	//uint8_t pad:3;

	/**
	 * Constructor.
	 * Zero initializes the data members.
	 */
	constexpr
	flags();

	/**
	 * Sets all flags based on the value.
	 * 
	 * @param val Instruction result
	 */
	constexpr
	void
	set_all(uint16_t val);
};


/**
 * Contains cpu registers.
 */
struct registers
{
	uint8_t a; ///< Register a
	uint8_t b; ///< Register b
	uint8_t c; ///< Register c
	uint8_t d; ///< Register d
	uint8_t e; ///< Register e
	uint8_t h; ///< Register h
	uint8_t l; ///< Register l

	/**
	 * Returns 16 bit value combined from hl registers.
	 * Format: h = 0xAB l = 0xCD hl = 0xABCD
	 * @return constexpr uint16_t 
	 */
	constexpr
	uint16_t
	hl() const;

	/**
	 * Constructor.
	 * Zero initializes the data members.
	 */
	constexpr
	registers();
};

/**
 * CPU
 */
class cpu
{
	registers regs_; ///< Registers

	uint16_t sp_; ///< Stact pointer
	uint16_t pc_; ///< Program counter

	std::vector<uint8_t> memory_; ///< Cpu memory

	flags flags_; ///< Cpu flags
	uint8_t int_enable_; ///< Enable interrupts

public:
	/**
	 * Constructor
	 * 
	 * @param memory Initialized cpu memory
	 */
	cpu(std::vector<uint8_t>&& memory);

	/**
	 * Step cpu by a single instruction.
	 */
	void
	step();
};

} // namespace atat
