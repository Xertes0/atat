#include <gtest/gtest.h>

#define private public
#include <atat/cpu.hh>
#undef private

#include <atat/opcodes.hh>

TEST(OpcodesTest, ArithmeticADD)
{
    uint8_t memory[] = {
        atat::opcodes::add_a,
        atat::opcodes::add_b,
        atat::opcodes::add_c,
        atat::opcodes::add_d,
        atat::opcodes::add_e,
        atat::opcodes::add_h,
        atat::opcodes::add_l,
        atat::opcodes::add_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 40);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 34);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 120);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 124);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 100);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 93);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 6);

    memory[8] = 100;
    cpu.regs_.a = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 110);
}

TEST(OpcodesTest, ArithmeticSUB)
{
    uint8_t memory[] = {
        atat::opcodes::sub_a,
        atat::opcodes::sub_b,
        atat::opcodes::sub_c,
        atat::opcodes::sub_d,
        atat::opcodes::sub_e,
        atat::opcodes::sub_h,
        atat::opcodes::sub_l,
        atat::opcodes::sub_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0);

    cpu.regs_.a = 24;
    cpu.regs_.b = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 4);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 40);

    cpu.regs_.a = 194;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 104);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 54);

    cpu.regs_.a = 100;
    cpu.regs_.h = 83;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 17);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 2);

    memory[8] = 10;
    cpu.regs_.a = 90;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 80);
}

TEST(OpcodesTest, ArithmeticINR)
{
    uint8_t memory[] = {
        atat::opcodes::inr_a,
        atat::opcodes::inr_b,
        atat::opcodes::inr_c,
        atat::opcodes::inr_d,
        atat::opcodes::inr_e,
        atat::opcodes::inr_h,
        atat::opcodes::inr_l,
        atat::opcodes::inr_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 21);

    cpu.regs_.b = 32;
    cpu.step();
    EXPECT_EQ(cpu.regs_.b, 33);

    cpu.regs_.c = 49;
    cpu.step();
    EXPECT_EQ(cpu.regs_.c, 50);

    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.d, 91);

    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.e, 24);

    cpu.regs_.h = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0);

    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.l, 3);

    memory[8] = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(memory[8], 11);
}

TEST(OpcodesTest, ArithmeticDCR)
{
    uint8_t memory[] = {
        atat::opcodes::dcr_a,
        atat::opcodes::dcr_b,
        atat::opcodes::dcr_c,
        atat::opcodes::dcr_d,
        atat::opcodes::dcr_e,
        atat::opcodes::dcr_h,
        atat::opcodes::dcr_l,
        atat::opcodes::dcr_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 19);

    cpu.regs_.b = 32;
    cpu.step();
    EXPECT_EQ(cpu.regs_.b, 31);

    cpu.regs_.c = 49;
    cpu.step();
    EXPECT_EQ(cpu.regs_.c, 48);

    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.d, 89);

    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.e, 22);

    cpu.regs_.h = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0xfe);

    cpu.regs_.l = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.l, 0xff);

    memory[8] = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(memory[8], 9);
}

TEST(OpcodesTest, ArithmeticCMP)
{
    uint8_t memory[] = {
        atat::opcodes::cmp_a,
        atat::opcodes::cmp_b,
        atat::opcodes::cmp_c,
        atat::opcodes::cmp_d,
        atat::opcodes::cmp_e,
        atat::opcodes::cmp_h,
        atat::opcodes::cmp_l,
        atat::opcodes::cmp_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 1);

    cpu.regs_.a = 83;
    cpu.regs_.b = 32;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 0);

    cpu.regs_.a = 49;
    cpu.regs_.c = 49;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 1);

    cpu.regs_.a = 49;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 0);

    cpu.regs_.a = 23;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 1);

    cpu.regs_.a = 0;
    cpu.regs_.h = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 0);

    cpu.regs_.a = 0xff;
    cpu.regs_.l = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 1);

    cpu.regs_.a = 10;
    memory[8] = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.flags_.z, 1);
}