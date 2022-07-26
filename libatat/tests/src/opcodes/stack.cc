#include <cstdint>
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

#define PUSH(A, B) \
    TEST(OpcodesTest, Stack_PUSH_##A) \
    { \
        uint8_t memory[] { \
            atat::opcodes::push_##A, \
            0, \
            0 \
        }; \
        auto cpu = atat::cpu{memory}; \
        cpu.sp_ = 3; \
        cpu.regs_.set_##A##B(0x1f27); \
        cpu.step(); \
        EXPECT_EQ(memory[1], 0x27); \
        EXPECT_EQ(memory[2], 0x1f); \
        EXPECT_EQ(cpu.sp_, 1); \
    }

PUSH(b, c)
PUSH(d, e)
PUSH(h, l)

TEST(OpcodesTest, Stack_PUSH_PSW)
{
    uint8_t memory[] {
        atat::opcodes::push_psw,
        0,
        0
    };

    auto cpu = atat::cpu{memory};
    cpu.sp_ = 3;

    cpu.flags_.s = 1;
    cpu.flags_.z = 0;
    cpu.flags_.p = 1;
    cpu.flags_.c = 0;
    cpu.regs_.a = 0x1f;
    cpu.step();
    EXPECT_EQ(memory[1], 0b10000110);
    EXPECT_EQ(memory[2], 0x1f);
    EXPECT_EQ(cpu.sp_, 1);
}