#include "atat/cpu/cpu.hh"

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
    pc_ = ((static_cast<uint16_t>(memory_[pc_ + 2]) << 8) | memory_[pc_ + 1]) - 1;

#define CALL() \
    uint16_t ret = pc_ + 3; \
    memory_[sp_-1] = static_cast<uint8_t>(ret >> 8); \
    memory_[sp_-2] = static_cast<uint8_t>(ret & 0xff); \
    sp_ -= 2; \
    pc_ = ((static_cast<uint16_t>(memory_[pc_+2]) << 8) | memory_[pc_+1]) - 1;

#define RET() \
    pc_ = ((static_cast<uint16_t>(memory_[sp_+1]) << 8) | memory_[sp_]) - 1; \
    sp_ += 2;

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

#define MAKE_JCR_COMBO(NAME, CALL) \
    case MAKE_JCR(NAME##nz, BI_Z,  CALL) \
    case MAKE_JCR(NAME##z,  BI_NZ, CALL) \
    case MAKE_JCR(NAME##nc, BI_C,  CALL) \
    case MAKE_JCR(NAME##c,  BI_NC, CALL) \
    case MAKE_JCR(NAME##po, BI_PE, CALL) \
    case MAKE_JCR(NAME##pe, BI_PO, CALL) \
    case MAKE_JCR(NAME##p,  BI_M,  CALL) \
    case MAKE_JCR(NAME##m,  BI_P,  CALL)

#define MOV(DST, SRC) \
    opcodes::mov_##DST##SRC: \
    { \
        regs_.DST = regs_.SRC; \
        break; \
    }

#define MOV_MEM(DST) \
    opcodes::mov_##DST##m: \
    { \
        regs_.DST = static_cast<uint8_t>(regs_.hl()); \
        break; \
    }

#define MOV_TMEM(SRC) \
    opcodes::mov_m##SRC: \
    { \
        regs_.set_hl(regs_.SRC); \
        break; \
    }

#define MOV_COMBO(DST) \
    case MOV(DST, b) \
    case MOV(DST, c) \
    case MOV(DST, d) \
    case MOV(DST, e) \
    case MOV(DST, h) \
    case MOV(DST, l) \
    case MOV_MEM(DST) \
    case MOV(DST, a)

#define MOV_MCOMBO() \
    case MOV_TMEM(b) \
    case MOV_TMEM(c) \
    case MOV_TMEM(d) \
    case MOV_TMEM(e) \
    case MOV_TMEM(h) \
    case MOV_TMEM(l) \
    case MOV_TMEM(a)

#define MOV_COMBO_COMBO() \
    MOV_COMBO(b) \
    MOV_COMBO(c) \
    MOV_COMBO(d) \
    MOV_COMBO(e) \
    MOV_COMBO(h) \
    MOV_COMBO(l) \
    MOV_MCOMBO() \
    MOV_COMBO(a)

namespace atat
{

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

        case opcodes::xthl:
        {
            auto hl   = regs_.hl();
            auto low  = memory_[sp_];
            auto high = memory_[sp_+1];

            memory_[sp_]   = hl & 0xff;
            memory_[sp_+1] = (hl >> 8) & 0xff;
            regs_.h = high;
            regs_.l = low;

            break;
        }

        case opcodes::jmp:
        {
            JMP();
            break;
        }

        case opcodes::call:
        {
            CALL();
            break;
        }

        case opcodes::ret:
        {
            RET();
            break;
        }

        MAKE_JCR_COMBO(j, JMP)
        MAKE_JCR_COMBO(c, CALL)
        MAKE_JCR_COMBO(r, RET)

        case opcodes::cmc:
        {
            flags_.c = !flags_.c;
            return;
        }

        case opcodes::stc:
        {
            flags_.c = 1;
            return;
        }

        case opcodes::di:
        {
            int_enable_ = false;
            break;
        }

        case opcodes::ei:
        {
            int_enable_ = true;
            break;
        }

        MOV_COMBO_COMBO()

    }
    ++pc_;
}

} // namespace atat