#include <gtest/gtest.h>

#include <atat/cpu.hh>

TEST(RegistersTest, ReadHL)
{
    auto regs = atat::registers{};

    regs.h = 0xab;
    regs.l = 0xcd;
    EXPECT_EQ(regs.hl(), 0xabcd);

    regs.h = 0x12;
    regs.l = 0x34;
    EXPECT_EQ(regs.hl(), 0x1234);
}

TEST(RegistersTest, SetHL)
{
    auto regs = atat::registers{};

    regs.set_hl(0xabcd);
    EXPECT_EQ(regs.h, 0xab);
    EXPECT_EQ(regs.l, 0xcd);

    regs.set_hl(0x1234);
    EXPECT_EQ(regs.h, 0x12);
    EXPECT_EQ(regs.l, 0x34);
}