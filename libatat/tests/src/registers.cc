#include <gtest/gtest.h>

#include <atat/cpu.hh>

TEST(RegistersTest, ReadBC)
{
    auto regs = atat::registers{};

    regs.b = 0xab;
    regs.c = 0xcd;
    EXPECT_EQ(regs.bc(), 0xabcd);

    regs.b = 0x12;
    regs.c = 0x34;
    EXPECT_EQ(regs.bc(), 0x1234);
}

TEST(RegistersTest, ReadDE)
{
    auto regs = atat::registers{};

    regs.d = 0xab;
    regs.e = 0xcd;
    EXPECT_EQ(regs.de(), 0xabcd);

    regs.d = 0x12;
    regs.e = 0x34;
    EXPECT_EQ(regs.de(), 0x1234);
}

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