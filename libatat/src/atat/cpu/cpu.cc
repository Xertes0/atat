#include "atat/cpu/cpu.hh"
#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

namespace atat
{

cpu::cpu(uint8_t* memory) :
    regs_{},
    sp_{},
    pc_{},
    memory_{memory},
    flags_{},
    int_enable_{} {}

} // namespace atat
