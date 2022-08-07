#include "atat/cpu/cpu.hh"

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

} // namespace atat
