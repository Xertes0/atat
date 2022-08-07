#pragma once

/**
 * @file disassembler.hh
 *
 * ROM disassembler.
 */

#include <cstdint>
#include <vector>
#include <string_view>

#include "atat/cpu/types.hh"

namespace atat
{

// TODO Rename to 'load_rom'??
/**
 * Allocate memory with rom.
 *
 * @param path Path to ROM file.
 * @return std::vector<uint8_t>
 */
std::vector<byte_t>
memory_with_rom(std::string_view path, word_t offset = 0);

} // namespace atat
