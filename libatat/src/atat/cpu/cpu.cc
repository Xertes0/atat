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
    sp{},
    pc{},
    memory{memory},
    flags{},
    int_enabled{0},
    in_cb{in_callback},
    out_cb{out_callback} {}

byte_t
cpu::peek(word_t count)
{
    return memory[pc + count];
}

void
cpu::skip_bytes(word_t count)
{
    pc += count;
}

void
cpu::generate_int(byte_t num)
{
    if(int_enabled == 0){ return; }
    int_enabled = 0;
    //assign<at_memory<apply<stack_ptr, static_value<word_t, 2>, minus>, word_t>, apply<program_counter, static_value<word_t, 3>, plus>>,
    //assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, minus>>,
    //assign<program_counter, following_data<word_t>>

    // stack pointer should not point to ram mirror but just in case
    memory[(sp - 2) & 0x3fff] = pc & 0xff;
    memory[(sp - 1) & 0x3fff] = (pc >> 8) & 0xff;
    sp -= 2;
    pc = 8*num;
}

} // namespace atat