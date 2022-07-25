#include <cstdint>
#include <gtest/gtest.h>

#define private public
#include <atat/cpu.hh>
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