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
    int_enabled_{1} {}

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

void
cpu::generate_int(byte_t num)
{
    if(int_enabled_ == 0){ return; }
    int_enabled_ = 0;
    //assign<at_memory<apply<stack_ptr, static_value<word_t, 2>, minus>, word_t>, apply<program_counter, static_value<word_t, 3>, plus>>,
    //assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, minus>>,
    //assign<program_counter, following_data<word_t>>
    memory[sp_ - 2] = pc_ & 0xff;
    memory[sp_ - 1] = (pc_ >> 8) & 0xff;
    sp_ -= 2;
    pc_ = 8*num;
}

} // namespace atat