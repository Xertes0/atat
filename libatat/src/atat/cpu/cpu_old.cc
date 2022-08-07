#include "atat/cpu/cpu.hh"
#include "atat/cpu/registers.hh"
#include "atat/cpu/flags.hh"

namespace atat
{

namespace old
{

uint16_t regs::bc() const
{
    return reg_bc::get(*reinterpret_cast<cpu*>(ptr));
}

uint16_t regs::de() const
{
    return reg_de::get(*reinterpret_cast<cpu*>(ptr));
}

uint16_t regs::hl() const
{
    return reg_hl::get(*reinterpret_cast<cpu*>(ptr));
}

void regs::set_bc(uint16_t value)
{
    reg_bc::set(*reinterpret_cast<cpu*>(ptr), value);
}

void regs::set_de(uint16_t value)
{
    reg_de::set(*reinterpret_cast<cpu*>(ptr), value);
}

void regs::set_hl(uint16_t value)
{
    reg_hl::set(*reinterpret_cast<cpu*>(ptr), value);
}

uint8_t flags::bits() const
{
    //return
    //    s << 7 |
    //    z << 6 |
    //    0 << 5 |
    //    0 << 4 | // auxillary flag is not implemented
    //    0 << 3 |
    //    p << 2 |
    //    1 << 1 |
    //    c;
    return flags_bits::get(*reinterpret_cast<cpu*>(ptr));
}

} // namespace atat::old

} // namespace atat