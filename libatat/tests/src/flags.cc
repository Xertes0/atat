#include <gtest/gtest.h>

#include <atat/cpu.hh>

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