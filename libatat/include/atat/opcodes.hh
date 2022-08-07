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
// http://textfiles.com/programming/8080.op
namespace opcodes
{

static constexpr inline uint8_t nop   {0x00};
static constexpr inline uint8_t lxi_b {0x01};
static constexpr inline uint8_t stax_b{0x02};
static constexpr inline uint8_t inx_b {0x03};
static constexpr inline uint8_t inr_b {0x04};
static constexpr inline uint8_t dcr_b {0x05};
static constexpr inline uint8_t mvi_b {0x06};
static constexpr inline uint8_t rlc   {0x07};
static constexpr inline uint8_t nop1  {0x08};
static constexpr inline uint8_t dad_b {0x09};
static constexpr inline uint8_t ldax_b{0x0a};
static constexpr inline uint8_t dcx_b {0x0b};
static constexpr inline uint8_t inr_c {0x0c};
static constexpr inline uint8_t dcr_c {0x0d};
static constexpr inline uint8_t mvi_c {0x0e};
static constexpr inline uint8_t rrc   {0x0f};
static constexpr inline uint8_t nop2  {0x10};
static constexpr inline uint8_t lxi_d {0x11};
static constexpr inline uint8_t stax_d{0x12};
static constexpr inline uint8_t inx_d {0x13};
static constexpr inline uint8_t inr_d {0x14};
static constexpr inline uint8_t dcr_d {0x15};
static constexpr inline uint8_t mvi_d {0x16};
static constexpr inline uint8_t ral   {0x17};
static constexpr inline uint8_t nop3  {0x18};
static constexpr inline uint8_t dad_d {0x19};
static constexpr inline uint8_t ldax_d{0x1a};
static constexpr inline uint8_t dcx_d {0x1b};
static constexpr inline uint8_t inr_e {0x1c};
static constexpr inline uint8_t dcr_e {0x1d};
static constexpr inline uint8_t mvi_e {0x1e};
static constexpr inline uint8_t rar   {0x1f};
static constexpr inline uint8_t nop4  {0x20};
static constexpr inline uint8_t lxi_h {0x21};
static constexpr inline uint8_t shld  {0x22};
static constexpr inline uint8_t inx_h {0x23};
static constexpr inline uint8_t inr_h {0x24};
static constexpr inline uint8_t dcr_h {0x25};
static constexpr inline uint8_t mvi_h {0x26};
//static constexpr inline uint8_t daa   {0x27};
static constexpr inline uint8_t nop5  {0x28};
static constexpr inline uint8_t dad_h {0x29};
static constexpr inline uint8_t lhld  {0x2a};
static constexpr inline uint8_t dcx_h {0x2b};
static constexpr inline uint8_t inr_l {0x2c};
static constexpr inline uint8_t dcr_l {0x2d};
static constexpr inline uint8_t mvi_l {0x2e};
//static constexpr inline uint8_t cma   {0x2f}; // auxillary flag not implemented
static constexpr inline uint8_t nop6  {0x30};
static constexpr inline uint8_t lxi_sp{0x31};
static constexpr inline uint8_t sta   {0x32};
static constexpr inline uint8_t inx_sp{0x33};
static constexpr inline uint8_t inr_m {0x34};
static constexpr inline uint8_t dcr_m {0x35};
static constexpr inline uint8_t mvi_m {0x36};
static constexpr inline uint8_t stc   {0x37};
static constexpr inline uint8_t nop7  {0x38};
static constexpr inline uint8_t dad_sp{0x39};
static constexpr inline uint8_t lda   {0x3a};
static constexpr inline uint8_t dcx_sp{0x3b};
static constexpr inline uint8_t inr_a {0x3c};
static constexpr inline uint8_t dcr_a {0x3d};
static constexpr inline uint8_t mvi_a {0x3e};
static constexpr inline uint8_t cmc   {0x3f};
static constexpr inline uint8_t mov_bb{0x40};
static constexpr inline uint8_t mov_bc{0x41};
static constexpr inline uint8_t mov_bd{0x42};
static constexpr inline uint8_t mov_be{0x43};
static constexpr inline uint8_t mov_bh{0x44};
static constexpr inline uint8_t mov_bl{0x45};
static constexpr inline uint8_t mov_bm{0x46};
static constexpr inline uint8_t mov_ba{0x47};
static constexpr inline uint8_t mov_cb{0x48};
static constexpr inline uint8_t mov_cc{0x49};
static constexpr inline uint8_t mov_cd{0x4a};
static constexpr inline uint8_t mov_ce{0x4b};
static constexpr inline uint8_t mov_ch{0x4c};
static constexpr inline uint8_t mov_cl{0x4d};
static constexpr inline uint8_t mov_cm{0x4e};
static constexpr inline uint8_t mov_ca{0x4f};
static constexpr inline uint8_t mov_db{0x50};
static constexpr inline uint8_t mov_dc{0x51};
static constexpr inline uint8_t mov_dd{0x52};
static constexpr inline uint8_t mov_de{0x53};
static constexpr inline uint8_t mov_dh{0x54};
static constexpr inline uint8_t mov_dl{0x55};
static constexpr inline uint8_t mov_dm{0x56};
static constexpr inline uint8_t mov_da{0x57};
static constexpr inline uint8_t mov_eb{0x58};
static constexpr inline uint8_t mov_ec{0x59};
static constexpr inline uint8_t mov_ed{0x5a};
static constexpr inline uint8_t mov_ee{0x5b};
static constexpr inline uint8_t mov_eh{0x5c};
static constexpr inline uint8_t mov_el{0x5d};
static constexpr inline uint8_t mov_em{0x5e};
static constexpr inline uint8_t mov_ea{0x5f};
static constexpr inline uint8_t mov_hb{0x60};
static constexpr inline uint8_t mov_hc{0x61};
static constexpr inline uint8_t mov_hd{0x62};
static constexpr inline uint8_t mov_he{0x63};
static constexpr inline uint8_t mov_hh{0x64};
static constexpr inline uint8_t mov_hl{0x65};
static constexpr inline uint8_t mov_hm{0x66};
static constexpr inline uint8_t mov_ha{0x67};
static constexpr inline uint8_t mov_lb{0x68};
static constexpr inline uint8_t mov_lc{0x69};
static constexpr inline uint8_t mov_ld{0x6a};
static constexpr inline uint8_t mov_le{0x6b};
static constexpr inline uint8_t mov_lh{0x6c};
static constexpr inline uint8_t mov_ll{0x6d};
static constexpr inline uint8_t mov_lm{0x6e};
static constexpr inline uint8_t mov_la{0x6f};
static constexpr inline uint8_t mov_mb{0x70};
static constexpr inline uint8_t mov_mc{0x71};
static constexpr inline uint8_t mov_md{0x72};
static constexpr inline uint8_t mov_me{0x73};
static constexpr inline uint8_t mov_mh{0x74};
static constexpr inline uint8_t mov_ml{0x75};
static constexpr inline uint8_t hlt   {0x76};
static constexpr inline uint8_t mov_ma{0x77};
static constexpr inline uint8_t mov_ab{0x78};
static constexpr inline uint8_t mov_ac{0x79};
static constexpr inline uint8_t mov_ad{0x7a};
static constexpr inline uint8_t mov_ae{0x7b};
static constexpr inline uint8_t mov_ah{0x7c};
static constexpr inline uint8_t mov_al{0x7d};
static constexpr inline uint8_t mov_am{0x7e};
static constexpr inline uint8_t mov_aa{0x7f};
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
static constexpr inline uint8_t rnz   {0xc0};
static constexpr inline uint8_t pop_b {0xc1};
static constexpr inline uint8_t jnz   {0xc2};
static constexpr inline uint8_t jmp   {0xc3};
static constexpr inline uint8_t cnz   {0xc4};
static constexpr inline uint8_t push_b{0xc5};
static constexpr inline uint8_t adi_d8{0xc6};

static constexpr inline uint8_t rz    {0xc8};
static constexpr inline uint8_t ret   {0xc9};
static constexpr inline uint8_t jz    {0xca};
static constexpr inline uint8_t nop8  {0xcb};
static constexpr inline uint8_t cz    {0xcc};
static constexpr inline uint8_t call  {0xcd};
static constexpr inline uint8_t aci_d8{0xce};

static constexpr inline uint8_t rnc   {0xd0};
static constexpr inline uint8_t pop_d {0xd1};
static constexpr inline uint8_t jnc   {0xd2};
static constexpr inline uint8_t out   {0xd3};
static constexpr inline uint8_t cnc   {0xd4};
static constexpr inline uint8_t push_d{0xd5};
static constexpr inline uint8_t sui_d8{0xd6};

static constexpr inline uint8_t rc    {0xd8};
static constexpr inline uint8_t nop9  {0xd9};
static constexpr inline uint8_t jc    {0xda};
static constexpr inline uint8_t in    {0xdb};
static constexpr inline uint8_t cc    {0xdc};
static constexpr inline uint8_t nop10 {0xdd};
static constexpr inline uint8_t sbi_d8{0xde};

static constexpr inline uint8_t rpo   {0xe0};
static constexpr inline uint8_t pop_h {0xe1};
static constexpr inline uint8_t jpo   {0xe2};
static constexpr inline uint8_t xthl  {0xe3};
static constexpr inline uint8_t cpo   {0xe4};
static constexpr inline uint8_t push_h{0xe5};
static constexpr inline uint8_t ani_d8{0xe6};

static constexpr inline uint8_t rpe   {0xe8};
static constexpr inline uint8_t pchl  {0xe9};
static constexpr inline uint8_t jpe   {0xea};
static constexpr inline uint8_t xchg  {0xeb};
static constexpr inline uint8_t cpe   {0xec};
static constexpr inline uint8_t nop11 {0xed};
static constexpr inline uint8_t xri_d8{0xee};

static constexpr inline uint8_t rp    {0xf0};
static constexpr inline uint8_t pop_psw{0xf1};
static constexpr inline uint8_t jp    {0xf2};
static constexpr inline uint8_t di    {0xf3};
static constexpr inline uint8_t cp    {0xf4};
static constexpr inline uint8_t push_psw{0xf5};
static constexpr inline uint8_t ori_d8{0xf6};

static constexpr inline uint8_t rm    {0xf8};
static constexpr inline uint8_t sphl  {0xf9};
static constexpr inline uint8_t jm    {0xfa};
static constexpr inline uint8_t ei    {0xfb};
static constexpr inline uint8_t cm    {0xfc};
static constexpr inline uint8_t nop12 {0xfd};
static constexpr inline uint8_t cpi_d8{0xfe};

} // namespace atat::opcodes

} // namespace atat
