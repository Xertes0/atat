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
    EXPECT_EQ(cpu.pc_, 3);
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
    EXPECT_EQ(memory[3], 23);
    EXPECT_EQ(cpu.pc_, 3);
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
        0x3,
        0x0,
        0x8a,
        0x23
    };

    auto cpu = atat::cpu{memory};

    cpu.step();
    EXPECT_EQ(cpu.regs_.l, 0x8a);
    EXPECT_EQ(cpu.regs_.h, 0x23);
    EXPECT_EQ(cpu.pc_, 3);
}

TEST(OpcodesTest, Transfer_SHLD)
{
    uint8_t memory[] {
        atat::opcodes::shld,
        3,
        0,
        0,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.l = 0x8a;
    cpu.regs_.h = 0x23;
    cpu.step();
    EXPECT_EQ(memory[3], 0x8a);
    EXPECT_EQ(memory[4], 0x23);
    EXPECT_EQ(cpu.pc_, 3);
}

#define LXI(A,B) \
    TEST(OpcodesTest, Transfer_LXI_##A) \
    { \
        uint8_t memory[] { \
            atat::opcodes::lxi_##A, \
            0xcf, \
            0x28 \
        }; \
        auto cpu = atat::cpu{memory}; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.A, 0x28); \
        EXPECT_EQ(cpu.regs_.B, 0xcf); \
    }

LXI(b, c)
LXI(d, e)
LXI(h, l)

TEST(OpcodesTest, Transfer_LXI_sp)
{
    uint8_t memory[] {
        atat::opcodes::lxi_sp,
        0xcf,
        0x28
    };
    auto cpu = atat::cpu{memory};
    cpu.step();
    EXPECT_EQ(cpu.sp_, 0x28cf);
}

TEST(OpcodesTest, Transfer_PCHL)
{
    uint8_t memory[] {
        atat::opcodes::pchl,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.set_hl(0xf4e5);
    cpu.step();
    EXPECT_EQ(cpu.pc_, 0xf4e5);
}

TEST(OpcodesTest, Transfer_XCHG)
{
    uint8_t memory[] {
        atat::opcodes::xchg,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.set_hl(0xf4e5);
    cpu.regs_.set_de(0x1234);
    cpu.step();
    EXPECT_EQ(cpu.regs_.hl(), 0x1234);
    EXPECT_EQ(cpu.regs_.de(), 0xf4e5);
}