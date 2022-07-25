#include "atat/cpu.hh"
#include "atat/exceptions.hh"
#include "atat/opcodes.hh"

#include <cstdint>
#include <limits>

namespace atat
{

flags::flags() :
    z{0},
    s{0},
    p{0},
    c{0}/*,
    a{0}*/ {}

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

void
flags::set_c(uint16_t val)
{
    c = (val > std::numeric_limits<uint8_t>::max())?1:0;
}

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

cpu::cpu(uint8_t* memory) :
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
        uint16_t val = static_cast<uint16_t>(memory_[regs_.hl()]) OP 1; \
        flags_.set_zsp(val); \
        memory_[regs_.hl()] OP##= 1; \
        break; \
    }

#define REG_CMP(REG) \
    opcodes::cmp_##REG: \
    { \
        flags_.set_zspc(static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(regs_.REG)); \
        break; \
    }

#define REG_CMP_MEM() \
    opcodes::cmp_m: \
    { \
        flags_.set_zspc(static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(memory_[regs_.hl()])); \
        break; \
    }

#define REG_LOG(NAME, REG, OP) \
    opcodes::NAME##_##REG: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(regs_.REG); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_LOG_MEM(NAME, OP) \
    opcodes::NAME##_m: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(memory_[regs_.hl()]); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ARI_CY(NAME,REG, OP) \
    opcodes::NAME##_##REG: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(regs_.REG) OP static_cast<uint16_t>(flags_.c); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define REG_ARI_CY_MEM(NAME, OP) \
    opcodes::NAME##_m: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(memory_[regs_.hl()]) OP static_cast<uint16_t>(flags_.c); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        break; \
    }

#define JMP() \
    pc_ = ((static_cast<uint16_t>(memory_[pc_ + 1]) << 8) | memory_[pc_ + 2]) - 1

// break if
#define BI_NZ() \
    if(flags_.z == 1) { pc_ += 2; break; }
#define BI_Z() \
    if(flags_.z == 0) { pc_ += 2; break; }
#define BI_NC() \
    if(flags_.c == 0) { pc_ += 2; break; }
#define BI_C() \
    if(flags_.c == 1) { pc_ += 2; break; }
#define BI_PO() \
    if(flags_.p == 0) { pc_ += 2; break; }
#define BI_PE() \
    if(flags_.p == 1) { pc_ += 2; break; }
#define BI_P() \
    if(flags_.s == 0) { pc_ += 2; break; }
#define BI_M() \
    if(flags_.s == 1) { pc_ += 2; break; }

#define MAKE_JCR(OPCODE, BI, INS) \
    opcodes::OPCODE: \
    { \
        BI(); \
        INS(); \
        break; \
    }

void
cpu::step()
{
    uint8_t const* op = memory_ + pc_;

    switch(*op)
    {
        default: {
            throw unimplemented_instruction_exception{*op};
            break;
        }

        case opcodes::nop: break;

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

        case REG_CMP(a)
        case REG_CMP(b)
        case REG_CMP(c)
        case REG_CMP(d)
        case REG_CMP(e)
        case REG_CMP(h)
        case REG_CMP(l)
        case REG_CMP_MEM()

        case REG_LOG(ana, a, &)
        case REG_LOG(ana, b, &)
        case REG_LOG(ana, c, &)
        case REG_LOG(ana, d, &)
        case REG_LOG(ana, e, &)
        case REG_LOG(ana, h, &)
        case REG_LOG(ana, l, &)
        case REG_LOG_MEM(ana, &)

        case REG_LOG(ora, a, |)
        case REG_LOG(ora, b, |)
        case REG_LOG(ora, c, |)
        case REG_LOG(ora, d, |)
        case REG_LOG(ora, e, |)
        case REG_LOG(ora, h, |)
        case REG_LOG(ora, l, |)
        case REG_LOG_MEM(ora, |)

        case REG_LOG(xra, a, ^)
        case REG_LOG(xra, b, ^)
        case REG_LOG(xra, c, ^)
        case REG_LOG(xra, d, ^)
        case REG_LOG(xra, e, ^)
        case REG_LOG(xra, h, ^)
        case REG_LOG(xra, l, ^)
        case REG_LOG_MEM(xra, ^)

        case opcodes::adi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::sui_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::cpi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            ++pc_;
            break;
        }

        case opcodes::ani_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) & static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::ori_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) | static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::xri_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) ^ static_cast<uint16_t>(*(op+1));
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case REG_ARI_CY(adc, a, +)
        case REG_ARI_CY(adc, b, +)
        case REG_ARI_CY(adc, c, +)
        case REG_ARI_CY(adc, d, +)
        case REG_ARI_CY(adc, e, +)
        case REG_ARI_CY(adc, h, +)
        case REG_ARI_CY(adc, l, +)
        case REG_ARI_CY_MEM(adc, +)

        case REG_ARI_CY(sbb, a, -)
        case REG_ARI_CY(sbb, b, -)
        case REG_ARI_CY(sbb, c, -)
        case REG_ARI_CY(sbb, d, -)
        case REG_ARI_CY(sbb, e, -)
        case REG_ARI_CY(sbb, h, -)
        case REG_ARI_CY(sbb, l, -)
        case REG_ARI_CY_MEM(sbb, -)

        case opcodes::aci_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) + static_cast<uint16_t>(*(op+1)) + static_cast<uint16_t>(flags_.c);
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::sbi_d8:
        {
            uint16_t val = static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(*(op+1)) - static_cast<uint16_t>(flags_.c);
            flags_.set_zspc(val);
            regs_.a = static_cast<uint8_t>(val);
            ++pc_;
            break;
        }

        case opcodes::dad_b:
        {
            uint16_t val = regs_.hl() + regs_.bc();
            flags_.set_c(val);
            regs_.set_hl(val);
            break;
        }

        case opcodes::dad_d:
        {
            uint16_t val = regs_.hl() + regs_.de();
            flags_.set_c(val);
            regs_.set_hl(val);
            break;
        }

        case opcodes::dad_h:
        {
            uint16_t val = regs_.hl() + regs_.hl();
            flags_.set_c(val);
            regs_.set_hl(val);
            break;
        }

        case opcodes::dad_sp:
        {
            uint16_t val = regs_.hl() + sp_;
            flags_.set_c(val);
            regs_.set_hl(val);
            break;
        }

        case opcodes::inx_b:
        {
            regs_.set_bc(regs_.bc() + 1);
            break;
        }

        case opcodes::inx_d:
        {
            regs_.set_de(regs_.de() + 1);
            break;
        }

        case opcodes::inx_h:
        {
            regs_.set_hl(regs_.hl() + 1);
            break;
        }

        case opcodes::inx_sp:
        {
            sp_ = sp_ + 1;
            break;
        }

        case opcodes::dcx_b:
        {
            regs_.set_bc(regs_.bc() - 1);
            break;
        }

        case opcodes::dcx_d:
        {
            regs_.set_de(regs_.de() - 1);
            break;
        }

        case opcodes::dcx_h:
        {
            regs_.set_hl(regs_.hl() - 1);
            break;
        }

        case opcodes::dcx_sp:
        {
            sp_ = sp_ - 1;
            break;
        }

        case opcodes::sphl:
        {
            sp_ = regs_.hl();
            break;
        }

        case opcodes::jmp:
        {
            JMP();
            break;
        }

        case MAKE_JCR(jnz, BI_Z, JMP)
        case MAKE_JCR(jz, BI_NZ, JMP)
        case MAKE_JCR(jnc, BI_C, JMP)
        case MAKE_JCR(jc, BI_NC, JMP)
        case MAKE_JCR(jpo, BI_PE, JMP)
        case MAKE_JCR(jpe, BI_PO, JMP)
        case MAKE_JCR(jp, BI_M, JMP)
        case MAKE_JCR(jm, BI_P, JMP)

    }
    ++pc_;
}

} // namespace atat
