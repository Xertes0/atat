#include <cstdint>
#include <gtest/gtest.h>

#define private public
#include <atat/cpu/cpu.hh>
#undef private

#include <atat/opcodes.hh>

TEST(OpcodesTest, Transfer_LDA)
{
    uint8_t memory[] {
        atat::opcodes::lda,
        3,
        0,
        0xef
    };

    auto cpu = atat::cpu{memory};

    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0xef);
}

TEST(OpcodesTest, Transfer_STA)
{
    uint8_t memory[] {
        atat::opcodes::sta,
        3,
        0,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 23);
}

TEST(OpcodesTest, Transfer_LDAX_B)
{
    uint8_t memory[] {
        atat::opcodes::ldax_b,
        0
    };

    auto cpu = atat::cpu{memory};

    memory[1] = 12;
    cpu.regs_.b = 0;
    cpu.regs_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 12);
}

TEST(OpcodesTest, Transfer_LDAX_D)
{
    uint8_t memory[] {
        atat::opcodes::ldax_d,
        0
    };

    auto cpu = atat::cpu{memory};

    memory[1] = 12;
    cpu.regs_.d = 0;
    cpu.regs_.e = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 12);
}

TEST(OpcodesTest, Transfer_STAX_B)
{
    uint8_t memory[] {
        atat::opcodes::stax_b,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 32;
    cpu.regs_.b = 0;
    cpu.regs_.c = 1;
    cpu.step();
    EXPECT_EQ(memory[1], 32);
}

TEST(OpcodesTest, Transfer_STAX_D)
{
    uint8_t memory[] {
        atat::opcodes::stax_d,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 32;
    cpu.regs_.d = 0;
    cpu.regs_.e = 1;
    cpu.step();
    EXPECT_EQ(memory[1], 32);
}

TEST(OpcodesTest, Transfer_LHLD)
{
    uint8_t memory[] {
        atat::opcodes::lhld,
        0x8a,
        0x23
    };

    auto cpu = atat::cpu{memory};

    cpu.step();
    EXPECT_EQ(cpu.regs_.l, 0x8a);
    EXPECT_EQ(cpu.regs_.h, 0x23);
}

TEST(OpcodesTest, Transfer_SHLD)
{
    uint8_t memory[] {
        atat::opcodes::shld,
        0,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.l = 0x8a;
    cpu.regs_.h = 0x23;
    cpu.step();
    EXPECT_EQ(memory[1], 0x8a);
    EXPECT_EQ(memory[2], 0x23);
}