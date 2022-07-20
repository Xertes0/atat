#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

namespace atat
{

std::vector<uint8_t>
disassemble(std::string_view path);

} // namespace atat
