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
 * Disassemble ROM file at given path.
 *
 * @param path Path to ROM file.
 * @return std::vector<uint8_t> 
 */
std::vector<uint8_t>
disassemble(std::string_view path);

} // namespace atat
