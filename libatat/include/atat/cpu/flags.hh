#pragma once

/**
 * @file flags.hh
 * 
 * Cpu flags.
 */

#include <cstdint>

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

} // namespace atat