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
	flags();

	/**
	 * Sets z,s,p,c flags based on the value.
	 * 
	 * @param val Instruction result
	 */
	void
	set_zspc(uint16_t val);

	/**
	 * Sets z,s,p flags based on the value.
	 * 
	 * @param val Instruction result
	 */
	void
	set_zsp(uint16_t val);

	/**
	 * Sets c flag based on the value.
	 * 
	 * @param val Instruction result
	 */
	void
	set_c(uint16_t val);
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
	 * Returns 16 bit value combined from bc registers.
	 * Format: b = 0xAB c = 0xCD bc = 0xABCD
	 * @return constexpr uint16_t 
	 */
	uint16_t
	bc() const;

	/**
	 * Returns 16 bit value combined from de registers.
	 * Format: d = 0xAB e = 0xCD de = 0xABCD
	 * @return constexpr uint16_t 
	 */
	uint16_t
	de() const;

	/**
	 * Returns 16 bit value combined from hl registers.
	 * Format: h = 0xAB l = 0xCD hl = 0xABCD
	 * @return constexpr uint16_t 
	 */
	uint16_t
	hl() const;

	/**
	 * Sets bc registers to value
	 * Format: bc = 0xABCD b = 0xAB c = 0xCD
	 *
	 * @param val Value
	 */
	void
	set_bc(uint16_t val);

	/**
	 * Sets de registers to value
	 * Format: de = 0xABCD d = 0xAB e = 0xCD
	 *
	 * @param val Value
	 */
	void
	set_de(uint16_t val);

	/**
	 * Sets hl registers to value
	 * Format: hl = 0xABCD h = 0xAB l = 0xCD
	 *
	 * @param val Value
	 */
	void
	set_hl(uint16_t val);

	/**
	 * Constructor.
	 * Zero initializes the data members.
	 */
	registers();
};

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
