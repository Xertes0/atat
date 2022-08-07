#include "atat/cpu/flags.hh"
#include "atat/opcodes.hh"
#include <gtest/gtest.h>

#include <atat/cpu/cpu.hh>
#include <atat/cpu/registers.hh>
#include <atat/cpu/flags.hh>

TEST(FlagsTest, ParityEven)
{
    atat::byte_t memory[] {
        atat::opcodes::add_b,
        atat::opcodes::add_b,
        atat::opcodes::add_b,
        atat::opcodes::add_b,
    };
    atat::cpu cpu{memory};
    atat::reg_b::set(cpu, 0);

    atat::reg_a::set(cpu, 0b1010);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_EVEN);

    atat::reg_a::set(cpu, 0b1001000);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_EVEN);

    atat::reg_a::set(cpu, 0b1111);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_EVEN);

    atat::reg_a::set(cpu, 0b11011);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_EVEN);
}

TEST(FlagsTest, ParityOdd)
{
    atat::byte_t memory[] {
        atat::opcodes::add_b,
        atat::opcodes::add_b,
        atat::opcodes::add_b,
        atat::opcodes::add_b,
    };
    atat::cpu cpu{memory};
    atat::reg_b::set(cpu, 0);

    atat::reg_a::set(cpu, 0b1110);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_ODD);

    atat::reg_a::set(cpu, 0b1000000);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_ODD);

    atat::reg_a::set(cpu, 0b1011);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_ODD);

    atat::reg_a::set(cpu, 0b01011);
    cpu.step();
    EXPECT_EQ(atat::flag_p::get(cpu), atat::PARITY_ODD);
}

TEST(FlagsTest, Bits)
{
    atat::cpu cpu{nullptr};
    atat::flag_s::set(cpu, 1);
    atat::flag_z::set(cpu, 0);
    atat::flag_p::set(cpu, 1);
    atat::flag_c::set(cpu, 0);
    EXPECT_EQ(atat::flags_bits::get(cpu), 0b10000110);
}

TEST(FlagsTest, SetBits)
{
    atat::cpu cpu{nullptr};
    atat::flags_bits::set(cpu, 0b10000110);

    EXPECT_EQ(atat::flag_s::get(cpu),  1);
    EXPECT_EQ(atat::flag_z::get(cpu),  0);
    EXPECT_EQ(atat::flag_p::get(cpu),  1);
    EXPECT_EQ(atat::flag_c::get(cpu),  0);
}