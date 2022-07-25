#include "atat/cpu/registers.hh"

namespace atat
{

registers::registers() :
    a{0},
    b{0},
    c{0},
    d{0},
    e{0},
    h{0},
    l{0} {}

uint16_t
registers::bc() const
{
    return (static_cast<uint16_t>(b) << 8) | static_cast<uint16_t>(c);
}

uint16_t
registers::de() const
{
    return (static_cast<uint16_t>(d) << 8) | static_cast<uint16_t>(e);
}

uint16_t
registers::hl() const
{
    return (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
}

void
registers::set_bc(uint16_t val)
{
    b = static_cast<uint8_t>(val >> 8);
    c = static_cast<uint8_t>(val);
}

void
registers::set_de(uint16_t val)
{
    d = static_cast<uint8_t>(val >> 8);
    e = static_cast<uint8_t>(val);
}

void
registers::set_hl(uint16_t val)
{
    h = static_cast<uint8_t>(val >> 8);
    l = static_cast<uint8_t>(val);
}

} // namespace atat