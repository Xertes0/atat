#include "atat/cpu/cpu.hh"
#include "atat/cpu/types.hh"

namespace atat
{

cpu::cpu(uint8_t* memory) :
    registers{},
    sp_{},
    pc_{},
    memory{memory},
    flags{},
    regs_{this},
    flags_{this},
    int_enabled_{} {}

byte_t
cpu::peek(word_t count)
{
    return memory[pc_ + count];
}

void
cpu::skip_bytes(word_t count)
{
    pc_ += count;
}

} // namespace atat