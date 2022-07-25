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
static constexpr inline uint8_t adc_b {0x88};
static constexpr inline uint8_t adc_c {0x89};
static constexpr inline uint8_t adc_d {0x8a};
static constexpr inline uint8_t adc_e {0x8b};
static constexpr inline uint8_t adc_h {0x8c};
static constexpr inline uint8_t adc_l {0x8d};
static constexpr inline uint8_t adc_m {0x8e};
static constexpr inline uint8_t adc_a {0x8f};
static constexpr inline uint8_t sub_b {0x90};
static constexpr inline uint8_t sub_c {0x91};
static constexpr inline uint8_t sub_d {0x92};
static constexpr inline uint8_t sub_e {0x93};
static constexpr inline uint8_t sub_h {0x94};
static constexpr inline uint8_t sub_l {0x95};
static constexpr inline uint8_t sub_m {0x96};
static constexpr inline uint8_t sub_a {0x97};
static constexpr inline uint8_t sbb_b {0x98};
static constexpr inline uint8_t sbb_c {0x99};
static constexpr inline uint8_t sbb_d {0x9a};
static constexpr inline uint8_t sbb_e {0x9b};
static constexpr inline uint8_t sbb_h {0x9c};
static constexpr inline uint8_t sbb_l {0x9d};
static constexpr inline uint8_t sbb_m {0x9e};
static constexpr inline uint8_t sbb_a {0x9f};
static constexpr inline uint8_t ana_b {0xa0};
static constexpr inline uint8_t ana_c {0xa1};
static constexpr inline uint8_t ana_d {0xa2};
static constexpr inline uint8_t ana_e {0xa3};
static constexpr inline uint8_t ana_h {0xa4};
static constexpr inline uint8_t ana_l {0xa5};
static constexpr inline uint8_t ana_m {0xa6};
static constexpr inline uint8_t ana_a {0xa7};
static constexpr inline uint8_t xra_b {0xa8};
static constexpr inline uint8_t xra_c {0xa9};
static constexpr inline uint8_t xra_d {0xaa};
static constexpr inline uint8_t xra_e {0xab};
static constexpr inline uint8_t xra_h {0xac};
static constexpr inline uint8_t xra_l {0xad};
static constexpr inline uint8_t xra_m {0xae};
static constexpr inline uint8_t xra_a {0xaf};
static constexpr inline uint8_t ora_b {0xb0};
static constexpr inline uint8_t ora_c {0xb1};
static constexpr inline uint8_t ora_d {0xb2};
static constexpr inline uint8_t ora_e {0xb3};
static constexpr inline uint8_t ora_h {0xb4};
static constexpr inline uint8_t ora_l {0xb5};
static constexpr inline uint8_t ora_m {0xb6};
static constexpr inline uint8_t ora_a {0xb7};
static constexpr inline uint8_t cmp_b {0xb8};
static constexpr inline uint8_t cmp_c {0xb9};
static constexpr inline uint8_t cmp_d {0xba};
static constexpr inline uint8_t cmp_e {0xbb};
static constexpr inline uint8_t cmp_h {0xbc};
static constexpr inline uint8_t cmp_l {0xbd};
static constexpr inline uint8_t cmp_m {0xbe};
static constexpr inline uint8_t cmp_a {0xbf};

static constexpr inline uint8_t adi_d8{0xc6};

static constexpr inline uint8_t sui_d8{0xd6};

static constexpr inline uint8_t ani_d8{0xe6};

static constexpr inline uint8_t xri_d8{0xee};

static constexpr inline uint8_t ori_d8{0xf6};

static constexpr inline uint8_t cpi_d8{0xfe};

} // namespace atat::opcodes

} // namespace atat
