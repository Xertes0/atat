#include "atat/cpu/cpu.hh"
#include "atat/opcodes.hh"
#include <cstdint>

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

#define REG_ARI_COMBO(NAME, OP) \
    case REG_ARI(NAME, a, OP) \
    case REG_ARI(NAME, b, OP) \
    case REG_ARI(NAME, c, OP) \
    case REG_ARI(NAME, d, OP) \
    case REG_ARI(NAME, e, OP) \
    case REG_ARI(NAME, h, OP) \
    case REG_ARI(NAME, l, OP) \
    case REG_ARI_MEM(NAME, OP)

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

#define REG_ONE_COMBO(NAME, OP) \
    case REG_ONE(NAME, a, OP) \
    case REG_ONE(NAME, b, OP) \
    case REG_ONE(NAME, c, OP) \
    case REG_ONE(NAME, d, OP) \
    case REG_ONE(NAME, e, OP) \
    case REG_ONE(NAME, h, OP) \
    case REG_ONE(NAME, l, OP) \
    case REG_ONE_MEM(NAME, OP)

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

#define REG_CMP_COMBO() \
    case REG_CMP(a) \
    case REG_CMP(b) \
    case REG_CMP(c) \
    case REG_CMP(d) \
    case REG_CMP(e) \
    case REG_CMP(h) \
    case REG_CMP(l) \
    case REG_CMP_MEM()

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

#define REG_MP_DATA(NAME, OP) \
    opcodes::NAME##_d8: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(*(op+1)); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        ++pc_; \
        break; \
    }

#define REG_LOG_COMBO(NAME, OP) \
    case REG_LOG(NAME, a, OP) \
    case REG_LOG(NAME, b, OP) \
    case REG_LOG(NAME, c, OP) \
    case REG_LOG(NAME, d, OP) \
    case REG_LOG(NAME, e, OP) \
    case REG_LOG(NAME, h, OP) \
    case REG_LOG(NAME, l, OP) \
    case REG_LOG_MEM(NAME, OP)

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

#define REG_ARI_CY_DATA(NAME, OP) \
    opcodes::NAME##_d8: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) OP static_cast<uint16_t>(*(op+1)) OP static_cast<uint16_t>(flags_.c); \
        flags_.set_zspc(val); \
        regs_.a = static_cast<uint8_t>(val); \
        ++pc_; \
        break; \
    }

#define REG_ARI_CY_COMBO(NAME, OP) \
    case REG_ARI_CY(NAME, a, OP) \
    case REG_ARI_CY(NAME, b, OP) \
    case REG_ARI_CY(NAME, c, OP) \
    case REG_ARI_CY(NAME, d, OP) \
    case REG_ARI_CY(NAME, e, OP) \
    case REG_ARI_CY(NAME, h, OP) \
    case REG_ARI_CY(NAME, l, OP) \
    case REG_ARI_CY_MEM(NAME, OP)

#define REG_DAD(NAME, FUN) \
    opcodes::dad_##NAME: \
    { \
        uint16_t val = regs_.hl() + regs_.FUN(); \
        flags_.set_c(val); \
        regs_.set_hl(val); \
        break; \
    }

#define REG_DAD_COMBO() \
    case REG_DAD(b, bc) \
    case REG_DAD(d, de) \
    case REG_DAD(h, hl) \
    case opcodes::dad_sp: \
    { \
        uint16_t val = regs_.hl() + sp_; \
        flags_.set_c(val); \
        regs_.set_hl(val); \
        break; \
    }

#define REG_IDC(NAME, FUN, OP) \
    opcodes::NAME: \
    { \
        regs_.set_##FUN(regs_.FUN() OP 1); \
        break; \
    }

#define REG_IDC_SP(NAME, OP) \
    opcodes::NAME: \
    { \
        sp_ = sp_ OP 1; \
        break; \
    }

#define REG_IDC_COMBO(NAME, OP) \
    case REG_IDC(NAME##_b, bc, OP) \
    case REG_IDC(NAME##_d, de, OP) \
    case REG_IDC(NAME##_h, hl, OP) \
    case REG_IDC_SP(NAME##_sp, OP)

#define REG_CPI() \
    opcodes::cpi_d8: \
    { \
        uint16_t val = static_cast<uint16_t>(regs_.a) - static_cast<uint16_t>(*(op+1)); \
        flags_.set_zspc(val); \
        ++pc_; \
        break; \
    }

#define REG_MVI(DST) \
    opcodes::mvi_##DST: \
    { \
        regs_.DST = memory_[pc_+1]; \
        ++pc_; \
        break; \
    }

#define REG_MVI_COMBO() \
    case REG_MVI(b) \
    case REG_MVI(c) \
    case REG_MVI(d) \
    case REG_MVI(e) \
    case REG_MVI(h) \
    case REG_MVI(l) \
    case REG_MVI(a) \
    case opcodes::mvi_m: \
    { \
        regs_.set_hl(memory_[pc_+1]); \
        ++pc_; \
        break; \
    }

#define LDAX(NAME, FUN) \
    opcodes::ldax_##NAME: \
    { \
        regs_.a = memory_[regs_.FUN()]; \
        break; \
    }

#define STAX(NAME, FUN) \
    opcodes::stax_##NAME: \
    { \
        memory_[regs_.FUN()] = regs_.a; \
        break; \
    }

#define LXI(A,B) \
    opcodes::lxi_##A: \
    { \
        regs_.A = memory_[pc_+2]; \
        regs_.B = memory_[pc_+1]; \
        pc_ += 2; \
        break; \
    }

#define LXI_COMBO() \
    case LXI(b, c) \
    case LXI(d, e) \
    case LXI(h, l) \
    case opcodes::lxi_sp: \
    { \
        sp_ = (static_cast<int16_t>(memory_[pc_+2]) << 8) | memory_[pc_+1]; \
        pc_ += 2; \
        break; \
    }

#define PUSH(A, B) \
    opcodes::push_##A: \
    { \
        memory_[sp_-1] = regs_.A; \
        memory_[sp_-2] = regs_.B; \
        sp_ -= 2; \
        break; \
    }

#define POP(A, B) \
    opcodes::pop_##A: \
    { \
        regs_.A = memory_[sp_+1]; \
        regs_.B = memory_[sp_]; \
        sp_ += 2; \
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

#define MAKE_JCR_COMBO(FULL, NAME, CALL) \
    case opcodes::FULL: \
    { \
        CALL(); \
        break; \
    } \
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
        default:
        {
            throw unimplemented_instruction_exception{*op};
            break;
        }

        case opcodes::out: break;
        case opcodes::in:  break;

        case opcodes::nop: break;
        case opcodes::nop1: break;
        case opcodes::nop2: break;
        case opcodes::nop3: break;
        case opcodes::nop4: break;
        case opcodes::nop5: break;
        case opcodes::nop6: break;
        case opcodes::nop7: break;
        case opcodes::nop8: break;
        case opcodes::nop9: break;
        case opcodes::nop10: break;
        case opcodes::nop11: break;
        case opcodes::nop12: break;

        REG_ARI_COMBO(add, +)
        REG_ARI_COMBO(sub, -)

        REG_ONE_COMBO(inr, +)
        REG_ONE_COMBO(dcr, -)

        REG_CMP_COMBO()

        REG_LOG_COMBO(ana, &)
        REG_LOG_COMBO(ora, |)
        REG_LOG_COMBO(xra, ^)

        case REG_CPI()

        case REG_MP_DATA(adi, +)
        case REG_MP_DATA(sui, -)
        case REG_MP_DATA(ani, &)
        case REG_MP_DATA(ori, |)
        case REG_MP_DATA(xri, ^)

        REG_ARI_CY_COMBO(adc, +)
        REG_ARI_CY_COMBO(sbb, -)

        case REG_ARI_CY_DATA(aci, +)
        case REG_ARI_CY_DATA(sbi, -)

        REG_DAD_COMBO()

        REG_IDC_COMBO(inx, +)
        REG_IDC_COMBO(dcx, -)

        MAKE_JCR_COMBO(jmp,  j, JMP)
        MAKE_JCR_COMBO(call, c, CALL)
        MAKE_JCR_COMBO(ret,  r, RET)

        MOV_COMBO_COMBO()

        REG_MVI_COMBO()

        LXI_COMBO()

        case LDAX(b, bc)
        case LDAX(d, de)

        case STAX(b, bc)
        case STAX(d, de)

        case PUSH(b, c);
        case PUSH(d, e);
        case PUSH(h, l);

        case opcodes::push_psw:
        {
            memory_[sp_-2] = flags_.bits();
            memory_[sp_-1] = regs_.a;
            sp_ -= 2;
            break;
        }

        case POP(b, c);
        case POP(d, e);
        case POP(h, l);

        case opcodes::pop_psw:
        {
            flags_.set_from_bits(memory_[sp_]);
            regs_.a = memory_[sp_+1];
            sp_ += 2;
            break;
        }

        case opcodes::pchl:
        {
            pc_ = regs_.hl() - 1;
            break;
        }

        case opcodes::xchg:
        {
            auto hl = regs_.hl();
            regs_.set_hl(regs_.de());
            regs_.set_de(hl);
            break;
        }

        case opcodes::lda:
        {
            regs_.a = memory_[(static_cast<uint16_t>(memory_[pc_+2]) << 8) | memory_[pc_+1]];
            pc_ += 2;
            break;
        }

        case opcodes::sta:
        {
            memory_[(static_cast<uint16_t>(memory_[pc_+2]) << 8) | memory_[pc_+1]] = regs_.a;
            pc_ += 2;
            break;
        }

        case opcodes::lhld:
        {
            regs_.l = memory_[pc_ + 1];
            regs_.h = memory_[pc_ + 2];
            pc_ += 2;
            break;
        }

        case opcodes::shld:
        {
            memory_[pc_ + 1] = regs_.l;
            memory_[pc_ + 2] = regs_.h;
            pc_ += 2;
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

        //case opcodes::cma:
        //{
        //    flags_.a = !flags_.a;
        //    return;
        //}

        case opcodes::ral:
        {
            auto prev_c = flags_.c;
            flags_.c = regs_.a >> 7;
            regs_.a <<= 1;
            regs_.a |= prev_c & 1;
            break;
        }

        case opcodes::rar:
        {
            auto prev_c = flags_.c;
            flags_.c = regs_.a & 1;
            regs_.a >>= 1;
            regs_.a |= (static_cast<uint8_t>(prev_c) << 7);
            break;
        }

        case opcodes::rlc:
        {
            flags_.c = (regs_.a >> 7) & 1;
            regs_.a <<= 1;
            regs_.a |= flags_.c;
            break;
        }

        case opcodes::rrc:
        {
            flags_.c = regs_.a & 1;
            regs_.a >>= 1;
            regs_.a |= static_cast<uint8_t>(flags_.c) << 7;
            break;
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

    }
    ++pc_;
}

} // namespace atat