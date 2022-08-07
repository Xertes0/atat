#include <gtest/gtest.h>

#define private public
#include <atat/cpu/cpu.hh>
#undef private

#include <atat/opcodes.hh>

TEST(OpcodesTest, Rotation_RAL)
{
    uint8_t memory[] {
        atat::opcodes::ral,
        atat::opcodes::ral
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a  = 0b10101010;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b01010100);
    EXPECT_EQ(cpu.flags_.c, 1);

    cpu.regs_.a  = 0b01010101;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b10101011);
    EXPECT_EQ(cpu.flags_.c, 0);
}

TEST(OpcodesTest, Rotation_RAR)
{
    uint8_t memory[] {
        atat::opcodes::rar,
        atat::opcodes::rar
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a  = 0b10101010;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b11010101);
    EXPECT_EQ(cpu.flags_.c, 0);

    cpu.regs_.a  = 0b01010101;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b00101010);
    EXPECT_EQ(cpu.flags_.c, 1);
}

TEST(OpcodesTest, Rotation_RLC)
{
    uint8_t memory[] {
        atat::opcodes::rlc,
        atat::opcodes::rlc
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a  = 0b10101010;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b01010101);
    EXPECT_EQ(cpu.flags_.c, 1);

    cpu.regs_.a  = 0b01010101;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b10101010);
    EXPECT_EQ(cpu.flags_.c, 0);
}

TEST(OpcodesTest, Rotation_RRC)
{
    uint8_t memory[] {
        atat::opcodes::rrc,
        atat::opcodes::rrc
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a  = 0b10101010;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b01010101);
    EXPECT_EQ(cpu.flags_.c, 0);

    cpu.regs_.a  = 0b01010101;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b10101010);
    EXPECT_EQ(cpu.flags_.c, 1);
}