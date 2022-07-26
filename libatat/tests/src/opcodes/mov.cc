#include <cstdint>
#include <gtest/gtest.h>

#define private public
#include <atat/cpu/cpu.hh>
#undef private

#include <atat/opcodes.hh>

#define MOV(DST, SRC) \
    TEST(OpcodesTest, MOV_##DST##SRC) \
    { \
        uint8_t memory[] = { \
            atat::opcodes::mov_##DST##SRC, \
            atat::opcodes::mov_##DST##SRC \
        }; \
        auto cpu = atat::cpu{memory}; \
        cpu.regs_.SRC = 83; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 83); \
        cpu.regs_.SRC = 12; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 12); \
    }

#define MOV_MEM(DST) \
    TEST(OpcodesTest, MOV_##DST##m) \
    { \
        uint8_t memory[] = { \
            atat::opcodes::mov_##DST##m, \
            atat::opcodes::mov_##DST##m \
        }; \
        auto cpu = atat::cpu{memory}; \
        cpu.regs_.set_hl(83); \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 83); \
        cpu.regs_.set_hl(12); \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 12); \
    }

#define MOV_TMEM(SRC) \
    TEST(OpcodesTest, MOV_m##SRC) \
    { \
        uint8_t memory[] = { \
            atat::opcodes::mov_m##SRC, \
            atat::opcodes::mov_m##SRC \
        }; \
        auto cpu = atat::cpu{memory}; \
        cpu.regs_.SRC = 83; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.hl(), 83); \
        cpu.regs_.SRC = 12; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.hl(), 12); \
    }

#define MOV_COMBO(DST) \
    MOV(DST, b) \
    MOV(DST, c) \
    MOV(DST, d) \
    MOV(DST, e) \
    MOV(DST, h) \
    MOV(DST, l) \
    MOV_MEM(DST) \
    MOV(DST, a)

#define MOV_MCOMBO() \
    MOV_TMEM(b) \
    MOV_TMEM(c) \
    MOV_TMEM(d) \
    MOV_TMEM(e) \
    MOV_TMEM(h) \
    MOV_TMEM(l) \
    MOV_TMEM(a)

#define MOV_COMBO_COMBO() \
    MOV_COMBO(b) \
    MOV_COMBO(c) \
    MOV_COMBO(d) \
    MOV_COMBO(e) \
    MOV_COMBO(h) \
    MOV_COMBO(l) \
    MOV_MCOMBO() \
    MOV_COMBO(a)

MOV_COMBO_COMBO()

#define MVI(DST) \
    TEST(OpcodesTest, MVI_##DST) \
    { \
        uint8_t memory[] { \
            atat::opcodes::mvi_##DST, \
            0, \
            atat::opcodes::mvi_##DST, \
            0 \
        }; \
        auto cpu = atat::cpu{memory}; \
        memory[1] = 0x23; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 0x23); \
        memory[3] = 0xf2; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.DST, 0xf2); \
    }

#define MVI_MEM() \
    TEST(OpcodesTest, MVI_m) \
    { \
        uint8_t memory[] { \
            atat::opcodes::mvi_m, \
            0, \
            atat::opcodes::mvi_m, \
            0 \
        }; \
        auto cpu = atat::cpu{memory}; \
        memory[1] = 0x23; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.hl(), 0x23); \
        memory[3] = 0xf2; \
        cpu.step(); \
        EXPECT_EQ(cpu.regs_.hl(), 0xf2); \
    }

#define MVI_COMBO() \
    MVI(b) \
    MVI(c) \
    MVI(d) \
    MVI(e) \
    MVI(h) \
    MVI(l) \
    MVI(a) \
    MVI_MEM()

MVI_COMBO()