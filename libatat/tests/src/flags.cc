#include <gtest/gtest.h>

#include <atat/cpu/flags.hh>

TEST(FlagsTest, ParityPositive)
{
    auto flags = atat::flags{};

    flags.set_zspc(0b1010);
    EXPECT_EQ(flags.p, 1);

    flags.set_zspc(0b1001000);
    EXPECT_EQ(flags.p, 1);

    flags.set_zspc(0b1111);
    EXPECT_EQ(flags.p, 1);

    flags.set_zspc(0b11011);
    EXPECT_EQ(flags.p, 1);
}

TEST(FlagsTest, ParityNegative)
{
    auto flags = atat::flags{};

    flags.set_zspc(0b0010);
    EXPECT_EQ(flags.p, 0);

    flags.set_zspc(0b1000000);
    EXPECT_EQ(flags.p, 0);

    flags.set_zspc(0b1011);
    EXPECT_EQ(flags.p, 0);

    flags.set_zspc(0b101111);
    EXPECT_EQ(flags.p, 0);
}

TEST(FlagsTest, Bits)
{
    auto flags = atat::flags{};
    flags.s = 1;
    flags.z = 0;
    flags.p = 1;
    flags.c = 0;
    EXPECT_EQ(flags.bits(), 0b10000110);
}

TEST(FlagsTest, SetBits)
{
    auto flags = atat::flags{};
    flags.set_from_bits(0b10000110);

    EXPECT_EQ(flags.s,  1);
    EXPECT_EQ(flags.z,  0);
    EXPECT_EQ(flags.p,  1);
    EXPECT_EQ(flags.c,  0);
}