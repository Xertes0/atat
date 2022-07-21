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
flags::set_zspc(uint16_t val)
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
void
flags::set_zsp(uint16_t val)
{
    z = (val == 0)?1:0;
    s = ((val & 0b1000000) == 1)?1:0;

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

constexpr
void
registers::set_hl(uint16_t val)
{
    h = static_cast<uint8_t>(val >> 8);
    l = static_cast<uint8_t>(val);
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
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ARI_MEM(NAME, OP) \
    opcodes::NAME##_m: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(memory_[regs_.hl()]); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ONE(NAME, REG, OP) \
    opcodes::NAME##_##REG: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.REG) OP 1; \
        flags_.set_zsp(val); \
        regs_.REG = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ONE_MEM(NAME, OP) \
    opcodes::NAME##_m: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.hl()) OP 1; \
        flags_.set_zsp(val); \
        regs_.set_hl(val); \
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

        case REG_ARI(sub, a, -)
        case REG_ARI(sub, b, -)
        case REG_ARI(sub, c, -)
        case REG_ARI(sub, d, -)
        case REG_ARI(sub, e, -)
        case REG_ARI(sub, h, -)
        case REG_ARI(sub, l, -)
        case REG_ARI_MEM(sub, -)

        case REG_ONE(inr, a, +)
        case REG_ONE(inr, b, +)
        case REG_ONE(inr, c, +)
        case REG_ONE(inr, d, +)
        case REG_ONE(inr, e, +)
        case REG_ONE(inr, h, +)
        case REG_ONE(inr, l, +)
        case REG_ONE_MEM(inr, +)

        case REG_ONE(dcr, a, -)
        case REG_ONE(dcr, b, -)
        case REG_ONE(dcr, c, -)
        case REG_ONE(dcr, d, -)
        case REG_ONE(dcr, e, -)
        case REG_ONE(dcr, h, -)
        case REG_ONE(dcr, l, -)
        case REG_ONE_MEM(dcr, -)

        case opcodes::adi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }
    }
    ++pc_;
}

} // namespace atat
