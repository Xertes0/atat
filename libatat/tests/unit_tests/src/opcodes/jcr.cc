#include <gtest/gtest.h>

#define private public
#include <atat/cpu/cpu.hh>
#undef private

#include <atat/opcodes.hh>

TEST(OpcodesTest, JCR_JMP)
{
    uint8_t memory[] = {
        atat::opcodes::jmp,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JNZ)
{
    uint8_t memory[] = {
        atat::opcodes::jnz,
        0xcd,
        0xab,
        atat::opcodes::jnz,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.z = 1;
    cpu.step();
    EXPECT_EQ(cpu.pc, 3);

    cpu.flags_.z = 0;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JZ)
{
    uint8_t memory[] = {
        atat::opcodes::jz,
        0xcd,
        0xab,
        atat::opcodes::jz,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.z = 0;
    cpu.step();
    EXPECT_EQ(cpu.pc, 3);

    cpu.flags_.z = 1;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JNC)
{
    uint8_t memory[] = {
        atat::opcodes::jnc,
        0xcd,
        0xab,
        atat::opcodes::jnc,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JC)
{
    uint8_t memory[] = {
        atat::opcodes::jc,
        0xcd,
        0xab,
        atat::opcodes::jc,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JPO)
{
    uint8_t memory[] = {
        atat::opcodes::jpo,
        0xcd,
        0xab,
        atat::opcodes::jpo,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.p = 1;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.p = 0;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JPE)
{
    uint8_t memory[] = {
        atat::opcodes::jpe,
        0xcd,
        0xab,
        atat::opcodes::jpe,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.p = 0;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.p = 1;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JP)
{
    uint8_t memory[] = {
        atat::opcodes::jp,
        0xcd,
        0xab,
        atat::opcodes::jp,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.s = 1;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.s = 0;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_JM)
{
    uint8_t memory[] = {
        atat::opcodes::jm,
        0xcd,
        0xab,
        atat::opcodes::jm,
        0xcd,
        0xab
    };

    auto cpu = atat::cpu{memory};

    cpu.flags_.s = 0;
    cpu.step();
    EXPECT_NE(cpu.pc, 0xabcd);

    cpu.flags_.s = 1;
    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
}

TEST(OpcodesTest, JCR_CALL)
{
    uint8_t memory[] = {
        atat::opcodes::sphl,
        atat::opcodes::call,
        0xcd,
        0xab,
        0,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.h = 0;
    cpu.regs_.l = 6;
    cpu.step();

    cpu.step();
    EXPECT_EQ(cpu.pc, 0xabcd);
    EXPECT_EQ(memory[4], 4);
    EXPECT_EQ(memory[5], 0);
    EXPECT_EQ(cpu.sp, 4);
}

// Depends on working CALL
TEST(OpcodesTest, JCR_RET)
{
    uint8_t memory[] = {
        atat::opcodes::sphl,
        atat::opcodes::call,
        6,
        0,
        0,
        0,
        atat::opcodes::ret,
        0,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.h = 0;
    cpu.regs_.l = 9;
    cpu.step();
    cpu.step();
    cpu.step();

    EXPECT_EQ(cpu.pc, 4);
    EXPECT_EQ(cpu.sp, 9);
}