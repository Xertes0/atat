#include "atat/cpu/cpu.hh"
#include "atat/cpu/types.hh"

namespace atat
{

cpu::cpu(
    byte_t* memory,
    std::optional<in_callback_t>  in_callback,
    std::optional<out_callback_t> out_callback
) :
    registers{},
    sp_{},
    pc_{},
    memory{memory},
    flags{},
    regs_{this},
    flags_{this},
    int_enabled_{0},
    in_cb{in_callback},
    out_cb{out_callback} {}

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

    // stack pointer should not point to ram mirror but just in case
    memory[(sp_ - 2) & 0x3fff] = pc_ & 0xff;
    memory[(sp_ - 1) & 0x3fff] = (pc_ >> 8) & 0xff;
    sp_ -= 2;
    pc_ = 8*num;
}

} // namespace atat