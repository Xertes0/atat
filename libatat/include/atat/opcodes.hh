#pragma once

#include <cstdint>

namespace atat
{

// For more info
// http://www.emulator101.com/reference/8080-by-opcode.html
namespace opcodes
{

static constexpr inline uint8_t nop   {0x00};
static constexpr inline uint8_t add_b {0x80};
static constexpr inline uint8_t add_c {0x81};
static constexpr inline uint8_t add_d {0x82};
static constexpr inline uint8_t add_e {0x83};
static constexpr inline uint8_t add_h {0x84};
static constexpr inline uint8_t add_l {0x85};
static constexpr inline uint8_t add_m {0x86};
static constexpr inline uint8_t add_a {0x87};
static constexpr inline uint8_t adi_d8{0xc6};

} // namespace atat::opcodes

} // namespace atat
