#pragma once

/**
 * @file disassembler.hh
 * 
 * ROM disassembler.
 */

#include <cstdint>
#include <vector>
#include <string_view>

namespace atat
{

// TODO Rename to 'load_rom'??
/**
 * Allocate memory with rom.
 *
 * @param path Path to ROM file.
 * @return std::vector<uint8_t>
 */
std::vector<uint8_t>
memory_with_rom(std::string_view path);

} // namespace atat
