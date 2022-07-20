#include "atat/cpu.hh"
#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

#include <cstdint>
#include <limits>

namespace atat
{

constexpr
flags::flags() :
    z{0},
    s{0},
    p{0},
    c{0}/*,
    a{0}*/ {}

constexpr
void
flags::set_from(uint16_t val)
{
    z = (val == 0)?1:0;
    s = ((val & 0b1000000) == 1)?1:0;
    p = (((val & 0xff) % 2) == 0)?1:0;
    c = (val > std::numeric_limits<uint8_t>::max())?1:0;
}

constexpr
registers::registers() :
    a{0},
    b{0},
    c{0},
    d{0},
    e{0},
    h{0},
    l{0} {}

constexpr
uint16_t
registers::hl() const
{
    return (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
}

cpu::cpu(std::vector<uint8_t>&& memory) :
    regs_{},
    sp_{},
    pc_{},
    memory_{memory},
    flags_{},
    int_enable_{} {}

#define REG_ARI(REG, OP) \
    opcodes::add_##REG: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(regs_.REG); \
        flags_.set_from(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

void
cpu::step()
{
    uint8_t* op = &memory_[pc_];

    switch(*op)
    {
        default: {
            throw unimplemented_instruction_exception{*op};
            break;
        }
        case opcodes::nop: break;
        case REG_ARI(a, +)
        case REG_ARI(b, +)
        case REG_ARI(c, +)
        case REG_ARI(d, +)
        case REG_ARI(e, +)
        case REG_ARI(h, +)
        case REG_ARI(l, +)
        case opcodes::add_m:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(memory_[regs_.hl()]);
            flags_.set_from(val);
            regs_.a = static_cast<uint8_t>(val);
            break;
        }
        case opcodes::adi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(*(op+1));
            flags_.set_from(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }
    }
    ++pc_;
}

} // namespace atat
