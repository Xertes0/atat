#include "atat/exceptions.hh"
#include "atat/opcodes.hh"
#include <gtest/gtest.h>

#include <atat/cpu/cpu.hh>
#include <atat/cpu/registers.hh>

//TEST(Exceptions, RomWriteException)
//{
//    atat::byte_t memory[] {
//        atat::opcodes::inr_m
//    };
//
//    atat::cpu cpu{memory};
//
//    atat::reg_hl::set(cpu, 0x01cf); // In read-only ROM
//    EXPECT_THROW(cpu.step(), atat::rom_write_exception);
//}