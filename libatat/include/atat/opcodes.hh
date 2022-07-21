#pragma once

/**
 * @file opcodes.hh
 *
 * Containes the cpu's opcodes.
 */

#include <cstdint>

namespace atat
{

// For more info
// http://www.emulator101.com/reference/8080-by-opcode.html
namespace opcodes
{

static constexpr inline uint8_t nop   {0x00};

static constexpr inline uint8_t inr_b {0x04};
static constexpr inline uint8_t dcr_b {0x05};

static constexpr inline uint8_t inr_c {0x0c};
static constexpr inline uint8_t dcr_c {0x0d};

static constexpr inline uint8_t inr_d {0x14};
static constexpr inline uint8_t dcr_d {0x15};

static constexpr inline uint8_t inr_e {0x1c};
static constexpr inline uint8_t dcr_e {0x1d};

static constexpr inline uint8_t inr_h {0x24};
static constexpr inline uint8_t dcr_h {0x25};

static constexpr inline uint8_t inr_l {0x2c};
static constexpr inline uint8_t dcr_l {0x2d};

static constexpr inline uint8_t inr_m {0x34};
static constexpr inline uint8_t dcr_m {0x35};

static constexpr inline uint8_t inr_a {0x3c};
static constexpr inline uint8_t dcr_a {0x3d};

static constexpr inline uint8_t add_b {0x80};
static constexpr inline uint8_t add_c {0x81};
static constexpr inline uint8_t add_d {0x82};
static constexpr inline uint8_t add_e {0x83};
static constexpr inline uint8_t add_h {0x84};
static constexpr inline uint8_t add_l {0x85};
static constexpr inline uint8_t add_m {0x86};
static constexpr inline uint8_t add_a {0x87};

static constexpr inline uint8_t sub_b {0x90};
static constexpr inline uint8_t sub_c {0x91};
static constexpr inline uint8_t sub_d {0x92};
static constexpr inline uint8_t sub_e {0x93};
static constexpr inline uint8_t sub_h {0x94};
static constexpr inline uint8_t sub_l {0x95};
static constexpr inline uint8_t sub_m {0x96};
static constexpr inline uint8_t sub_a {0x97};

static constexpr inline uint8_t cmp_b {0xb8};
static constexpr inline uint8_t cmp_c {0xb9};
static constexpr inline uint8_t cmp_d {0xba};
static constexpr inline uint8_t cmp_e {0xbb};
static constexpr inline uint8_t cmp_h {0xbc};
static constexpr inline uint8_t cmp_l {0xbd};
static constexpr inline uint8_t cmp_m {0xbe};
static constexpr inline uint8_t cmp_a {0xbf};

static constexpr inline uint8_t adi_d8{0xc6};

} // namespace atat::opcodes

} // namespace atat
