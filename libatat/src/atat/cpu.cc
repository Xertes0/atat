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
flags::set_all(uint16_t val)
{
    z = (val == 0)?1:0;
    s = ((val & 0b1000000) == 1)?1:0;
    c = (val > std::numeric_limits<uint8_t>::max())?1:0;

    uint8_t count{0};
    for(uint8_t i=0;i<8;i++) {
        count += ((val & (1 << i)) > 0)?1:0;
    }
    p = (count%2 == 0)?1:0;
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

#define REG_ARI(NAME,REG, OP) \
    opcodes::NAME##_##REG: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(regs_.REG); \
        flags_.set_all(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ARI_MEM(NAME, OP) \
    opcodes::NAME##_m: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(memory_[regs_.hl()]); \
        flags_.set_all(val); \
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
        // NOP
        case opcodes::nop: break;

        // Arithmetic
        case REG_ARI(add, a, +)
        case REG_ARI(add, b, +)
        case REG_ARI(add, c, +)
        case REG_ARI(add, d, +)
        case REG_ARI(add, e, +)
        case REG_ARI(add, h, +)
        case REG_ARI(add, l, +)
        case REG_ARI_MEM(add, +)

        case opcodes::adi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(*(op+1));
            flags_.set_all(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }
    }
    ++pc_;
}

} // namespace atat
