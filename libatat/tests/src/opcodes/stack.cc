#include <gtest/gtest.h>

#define private public
#include <atat/cpu/cpu.hh>
#undef private

#include <atat/opcodes.hh>

TEST(OpcodesTest, Stack_SPHL)
{
    uint8_t memory[] = {
        atat::opcodes::sphl,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.h = 0xab;
    cpu.regs_.l = 0xcd;
    cpu.step();
    EXPECT_EQ(cpu.sp_, 0xabcd);
}

// Depends on working SPHL
TEST(OpcodesTest, Stack_XTHL)
{
    uint8_t memory[] = {
        atat::opcodes::sphl,
        atat::opcodes::xthl,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.h = 0;
    cpu.regs_.l = 2;
    cpu.step();

    cpu.regs_.h = 0x34;
    cpu.regs_.l = 0x12;
    cpu.step();

    EXPECT_EQ(cpu.regs_.h, 0xab);
    EXPECT_EQ(cpu.regs_.l, 0xcd);
    EXPECT_EQ(memory[3],   0x34);
    EXPECT_EQ(memory[2],   0x12);
}