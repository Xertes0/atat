#pragma once

/**
 * @file registers.hh
 * 
 * Cpu registers.
 */

#include <cstdint>

namespace atat
{

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
	uint8_t h; ///< Register h (high)
	uint8_t l; ///< Register l (low)

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

} // namespace atat