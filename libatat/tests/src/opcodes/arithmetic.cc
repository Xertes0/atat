#include <gtest/gtest.h>

#include <atat/cpu/cpu.hh>
#include <atat/opcodes.hh>

// TODO Check flags

TEST(OpcodesTest, Arithmetic_ADD)
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

TEST(OpcodesTest, Arithmetic_SUB)
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

TEST(OpcodesTest, Arithmetic_INR)
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

TEST(OpcodesTest, Arithmetic_DCR)
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

TEST(OpcodesTest, Arithmetic_CMP)
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

TEST(OpcodesTest, Arithmetic_ANA)
{
    uint8_t memory[] = {
        atat::opcodes::ana_a,
        atat::opcodes::ana_b,
        atat::opcodes::ana_c,
        atat::opcodes::ana_d,
        atat::opcodes::ana_e,
        atat::opcodes::ana_h,
        atat::opcodes::ana_l,
        atat::opcodes::ana_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 20);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 14 & 20);

    cpu.regs_.a = 81;
    cpu.regs_.c = 43;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 81 & 43);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 34 & 90);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 77 & 23);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 10 & 83);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 4 & 2);

    memory[8]   = 0b0011;
    cpu.regs_.a = 0b1001;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 1);
}

TEST(OpcodesTest, Arithmetic_ORA)
{
    uint8_t memory[] = {
        atat::opcodes::ora_a,
        atat::opcodes::ora_b,
        atat::opcodes::ora_c,
        atat::opcodes::ora_d,
        atat::opcodes::ora_e,
        atat::opcodes::ora_h,
        atat::opcodes::ora_l,
        atat::opcodes::ora_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 20);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 14 | 20);

    cpu.regs_.a = 81;
    cpu.regs_.c = 43;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 81 | 43);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 34 | 90);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 77 | 23);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 10 | 83);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 4 | 2);

    memory[8]   = 0b0011;
    cpu.regs_.a = 0b1001;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0b1011);
}

TEST(OpcodesTest, Arithmetic_XRA)
{
    uint8_t memory[] = {
        atat::opcodes::xra_a,
        atat::opcodes::xra_b,
        atat::opcodes::xra_c,
        atat::opcodes::xra_d,
        atat::opcodes::xra_e,
        atat::opcodes::xra_h,
        atat::opcodes::xra_l,
        atat::opcodes::xra_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 14 ^ 20);

    cpu.regs_.a = 81;
    cpu.regs_.c = 43;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 81 ^ 43);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 34 ^ 90);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 77 ^ 23);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 10 ^ 83);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 4 ^ 2);

    memory[8]   = 0b0011;
    cpu.regs_.a = 0b1001;
    cpu.regs_.h = 0;
    cpu.regs_.l = 8;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0b1010);
}

TEST(OpcodesTest, Arithmetic_ADI)
{
    uint8_t memory[] = {
        atat::opcodes::adi_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 34;
    memory[1]   = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 57);
    EXPECT_EQ(cpu.pc_, 2);
}

TEST(OpcodesTest, Arithmetic_SUI)
{
    uint8_t memory[] = {
        atat::opcodes::sui_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 34;
    memory[1]   = 23;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 11);
    EXPECT_EQ(cpu.pc_, 2);
}

TEST(OpcodesTest, Arithmetic_CPI)
{
    uint8_t memory[] = {
        atat::opcodes::cpi_d8,
        0,
        atat::opcodes::cpi_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 34;
    memory[1]   = 34;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  34);
    EXPECT_EQ(cpu.flags_.z, 1);
    EXPECT_EQ(cpu.pc_, 2);

    cpu.regs_.a = 23;
    memory[3]   = 50;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  23);
    EXPECT_EQ(cpu.flags_.z, 0);
    EXPECT_EQ(cpu.pc_, 4);
}

TEST(OpcodesTest, Arithmetic_ANI)
{
    uint8_t memory[] = {
        atat::opcodes::ani_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 0b10011;
    memory[1]   = 0b11001;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b10001);
    EXPECT_EQ(cpu.pc_, 2);
}

TEST(OpcodesTest, Arithmetic_ORI)
{
    uint8_t memory[] = {
        atat::opcodes::ori_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 0b10011;
    memory[1]   = 0b11001;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b11011);
    EXPECT_EQ(cpu.pc_, 2);
}

TEST(OpcodesTest, Arithmetic_XRI)
{
    uint8_t memory[] = {
        atat::opcodes::xri_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 0b10011;
    memory[1]   = 0b11001;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a,  0b01010);
    EXPECT_EQ(cpu.pc_, 2);
}

TEST(OpcodesTest, Arithmetic_ADC)
{
    uint8_t memory[] = {
        atat::opcodes::adc_a,
        atat::opcodes::adc_a,
        atat::opcodes::adc_b,
        atat::opcodes::adc_b,
        atat::opcodes::adc_c,
        atat::opcodes::adc_c,
        atat::opcodes::adc_d,
        atat::opcodes::adc_d,
        atat::opcodes::adc_e,
        atat::opcodes::adc_e,
        atat::opcodes::adc_h,
        atat::opcodes::adc_h,
        atat::opcodes::adc_l,
        atat::opcodes::adc_l,
        atat::opcodes::adc_m,
        atat::opcodes::adc_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 41);

    cpu.regs_.a = 20;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 40);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 35);

    cpu.regs_.a = 14;
    cpu.regs_.b = 20;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 34);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 121);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 120);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 125);

    cpu.regs_.a = 34;
    cpu.regs_.d = 90;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 124);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 101);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 100);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 94);

    cpu.regs_.a = 10;
    cpu.regs_.h = 83;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 93);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 7);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 6);

    memory[16] = 100;
    cpu.regs_.a = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 16;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 111);

    memory[16] = 100;
    cpu.regs_.a = 10;
    cpu.regs_.h = 0;
    cpu.regs_.l = 16;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 110);
}

TEST(OpcodesTest, Arithmetic_SBB)
{
    uint8_t memory[] = {
        atat::opcodes::sbb_a,
        atat::opcodes::sbb_a,
        atat::opcodes::sbb_b,
        atat::opcodes::sbb_b,
        atat::opcodes::sbb_c,
        atat::opcodes::sbb_c,
        atat::opcodes::sbb_d,
        atat::opcodes::sbb_d,
        atat::opcodes::sbb_e,
        atat::opcodes::sbb_e,
        atat::opcodes::sbb_h,
        atat::opcodes::sbb_h,
        atat::opcodes::sbb_l,
        atat::opcodes::sbb_l,
        atat::opcodes::sbb_m,
        atat::opcodes::sbb_m,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0);

    cpu.regs_.a = 20;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 0xff);

    cpu.regs_.a = 24;
    cpu.regs_.b = 20;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 4);

    cpu.regs_.a = 24;
    cpu.regs_.b = 20;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 3);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 40);

    cpu.regs_.a = 80;
    cpu.regs_.c = 40;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 39);

    cpu.regs_.a = 194;
    cpu.regs_.d = 90;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 104);

    cpu.regs_.a = 194;
    cpu.regs_.d = 90;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 103);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 54);

    cpu.regs_.a = 77;
    cpu.regs_.e = 23;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 53);

    cpu.regs_.a = 100;
    cpu.regs_.h = 83;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 17);

    cpu.regs_.a = 100;
    cpu.regs_.h = 83;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 16);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 2);

    cpu.regs_.a = 4;
    cpu.regs_.l = 2;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 1);

    memory[16] = 10;
    cpu.regs_.a = 90;
    cpu.regs_.h = 0;
    cpu.regs_.l = 16;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 80);

    memory[16] = 10;
    cpu.regs_.a = 90;
    cpu.regs_.h = 0;
    cpu.regs_.l = 16;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 79);
}

TEST(OpcodesTest, Arithmetic_ACI)
{
    uint8_t memory[] = {
        atat::opcodes::aci_d8,
        0,
        atat::opcodes::aci_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    memory[1] = 10;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 30);

    cpu.regs_.a = 20;
    memory[3] = 10;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 31);
}

TEST(OpcodesTest, Arithmetic_SBI)
{
    uint8_t memory[] = {
        atat::opcodes::sbi_d8,
        0,
        atat::opcodes::sbi_d8,
        0
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.a = 20;
    memory[1] = 10;
    cpu.flags_.c = 0;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 10);

    cpu.regs_.a = 20;
    memory[3] = 10;
    cpu.flags_.c = 1;
    cpu.step();
    EXPECT_EQ(cpu.regs_.a, 9);
}

TEST(OpcodesTest, Arithmetic_DAD)
{
    uint8_t memory[] = {
        atat::opcodes::dad_b,
        atat::opcodes::dad_d,
        atat::opcodes::dad_h,
        atat::opcodes::dad_sp,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.h = 0b00000011;
    cpu.regs_.l = 0b11111111;
    cpu.regs_.b = 0b00000000;
    cpu.regs_.c = 0b00000001;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0b00000100);
    EXPECT_EQ(cpu.regs_.l, 0);

    cpu.regs_.h = 0b00000011;
    cpu.regs_.l = 0b11011111;
    cpu.regs_.d = 0b10000000;
    cpu.regs_.e = 0b00100001;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0b10000100);
    EXPECT_EQ(cpu.regs_.l, 0);

    cpu.regs_.h = 0b00000000;
    cpu.regs_.l = 0b00001111;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0);
    EXPECT_EQ(cpu.regs_.l, 30);

    cpu.regs_.h = 0b00000011;
    cpu.regs_.l =         0b00001011;
    cpu.sp_     = 0b0010000000000101;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0b00100011);
    EXPECT_EQ(cpu.regs_.l, 0b00010000);
}

TEST(OpcodesTest, Arithmetic_INX)
{
    uint8_t memory[] = {
        atat::opcodes::inx_b,
        atat::opcodes::inx_d,
        atat::opcodes::inx_h,
        atat::opcodes::inx_sp,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.b = 0xab;
    cpu.regs_.c = 0xcd;
    cpu.step();
    EXPECT_EQ(cpu.regs_.b, 0xab);
    EXPECT_EQ(cpu.regs_.c, 0xce);

    cpu.regs_.d = 0xff;
    cpu.regs_.e = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.d, 0);
    EXPECT_EQ(cpu.regs_.e, 0);

    cpu.regs_.h = 0x12;
    cpu.regs_.l = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0x13);
    EXPECT_EQ(cpu.regs_.l, 0);

    cpu.sp_ = 1234;
    cpu.step();
    EXPECT_EQ(cpu.sp_, 1235);
}

TEST(OpcodesTest, Arithmetic_DCX)
{
    uint8_t memory[] = {
        atat::opcodes::dcx_b,
        atat::opcodes::dcx_d,
        atat::opcodes::dcx_h,
        atat::opcodes::dcx_sp,
    };

    auto cpu = atat::cpu{memory};

    cpu.regs_.b = 0xab;
    cpu.regs_.c = 0xcd;
    cpu.step();
    EXPECT_EQ(cpu.regs_.b, 0xab);
    EXPECT_EQ(cpu.regs_.c, 0xcc);

    cpu.regs_.d = 0xff;
    cpu.regs_.e = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.d, 0xff);
    EXPECT_EQ(cpu.regs_.e, 0xfe);

    cpu.regs_.h = 0x12;
    cpu.regs_.l = 0xff;
    cpu.step();
    EXPECT_EQ(cpu.regs_.h, 0x12);
    EXPECT_EQ(cpu.regs_.l, 0xfe);

    cpu.sp_ = 1234;
    cpu.step();
    EXPECT_EQ(cpu.sp_, 1233);
}