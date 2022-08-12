#include "atat/cpu/cpu.hh"
#include "atat/cpu/registers.hh"
#include "atat/cpu/name_to_index.hh"
#include "atat/cpu/registers.hh"
#include "atat/cpu/flags.hh"
#include "atat/cpu/types.hh"
#include "atat/exceptions.hh"

#include <array>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <functional>
#include <type_traits>

namespace atat
{

template<class A, class B, class Func, bool SetTmp = false>
struct apply
{
    [[nodiscard]]
    static
    constexpr
    word_t
    get(cpu& ctx)
    {
        word_t value = Func{}(static_cast<word_t>(A::get(ctx)), static_cast<word_t>(B::get(ctx)));
        if constexpr(SetTmp) {
            ctx.step_tmp = value;
        }
        return value;
    }

    static
    constexpr
    void
    exec(cpu& ctx)
    {
        word_t value = Func{}(static_cast<word_t>(A::get(ctx)), static_cast<word_t>(B::get(ctx)));
        if constexpr(SetTmp) {
            ctx.step_tmp = value;
        }
    }
};

using plus    = std::plus<word_t>;
using minus   = std::minus<word_t>;
using bit_and = std::bit_and<word_t>;
using bit_or  = std::bit_or<word_t>;
using bit_xor = std::bit_xor<word_t>;

template<class Dst, class Src, bool SetTmp = false>
struct assign
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        auto value = Src::get(ctx);
        if constexpr(SetTmp) {
            ctx.step_tmp = value;
        }
        Dst::set(ctx, value);
    }
};

template<class Func, class... Rest>
struct chain
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        Func::exec(ctx);
        if constexpr(sizeof...(Rest) > 0) {
            chain<Rest...>::exec(ctx);
        }
    }
};

struct step_tmp_value
{
    [[nodiscard]]
    static
    constexpr
    word_t
    get(cpu& ctx)
    {
        return ctx.step_tmp;
    };
};

template<class Value, class First, class... Rest>
struct set_flags_from_value
{
    template<class Flag, class... Flags>
    static
    constexpr
    void set_flag(cpu& ctx)
    {
        word_t val{Value::get(ctx)};
        if constexpr(std::is_same_v<Flag, flag_z>) {
            flag_z::set(ctx, ((val & 0xff) == 0)?1:0);
        } else if constexpr(std::is_same_v<Flag, flag_s>) {
            flag_s::set(ctx, ((val & 0b10000000) != 0)?1:0);
        } else if constexpr(std::is_same_v<Flag, flag_c>) {
            flag_c::set(ctx, (val > std::numeric_limits<byte_t>::max())?1:0);
        } else if constexpr(std::is_same_v<Flag, flag_p>) {
            //val &= 0b01111111; // Ignore sign bit
            //byte_t count{0};
            //for(byte_t i{0}; i<std::numeric_limits<byte_t>::digits; ++i) {
            //    count += ((val & (1 << i)) > 0)?1:0;
            //}
            //flag_p::set(ctx, (count%2 == 0)?PARITY_EVEN:PARITY_ODD);

            val &= 0xff;
            byte_t count{0};
            while(val > 0) {
                if(val & 0x1) {
                    ++count;
                }
                val >>= 1;
            }
            flag_p::set(ctx, (count&0x1)?0:1);
        } else {
            int a = *((volatile int*)0);
        }

        if constexpr(sizeof...(Flags) > 0) {
            set_flag<Flags...>(ctx);
        }
    }

    static
    constexpr
    void exec(cpu& ctx)
    {
        set_flag<First, Rest...>(ctx);
    }
};

[[nodiscard]]
static
constexpr
word_t apply_mirror(word_t loc)
{
    return loc & 0x3fff;
}

template<class Location, class Type>
struct at_memory;

template<class Location>
struct at_memory<Location, byte_t>
{
    [[nodiscard]]
    static
    constexpr
    byte_t
    get(cpu& ctx)
    {
        return ctx.memory[apply_mirror(Location::get(ctx))];
    }

    static
    constexpr
    void
    set(cpu& ctx, byte_t value)
    {
        #ifdef MEMORY_CHECKS
        if(apply_mirror(Location::get(ctx)) < ROM_SIZE) {
            throw rom_write_exception{};
        }
        #endif
        ctx.memory[apply_mirror(Location::get(ctx))] = value;
    }
};

template<class Location>
struct at_memory<Location, word_t>
{
    [[nodiscard]]
    static
    constexpr
    word_t
    get(cpu& ctx)
    {
        return (static_cast<word_t>(ctx.memory[apply_mirror(Location::get(ctx) + 1)]) << 8) | ctx.memory[apply_mirror(Location::get(ctx))];
    }

    static
    constexpr
    void
    set(cpu& ctx, word_t value)
    {
        #ifdef MEMORY_CHECKS
        if(apply_mirror(Location::get(ctx) + 1) < ROM_SIZE) {
            throw rom_write_exception{};
        }
        #endif
        ctx.memory[apply_mirror(Location::get(ctx) + 1)] = (value >> 8) & 0xff;
        ctx.memory[apply_mirror(Location::get(ctx))] = value & 0xff;
    }
};

template<std::size_t N>
struct step
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        ctx.pc += N;
    }
};

template<class Type, Type Value>
struct static_value
{
    using value_t = Type;

    [[nodiscard]]
    static
    constexpr
    value_t get(cpu& ctx)
    {
        return Value;
    }
};

template<class Type>
struct following_data;

template<>
struct following_data<byte_t>
{
    [[nodiscard]]
    static
    constexpr
    byte_t get(cpu& ctx)
    {
        return ctx.memory[ctx.pc + 1];
    }
};

template<>
struct following_data<word_t>
{
    [[nodiscard]]
    static
    constexpr
    word_t get(cpu& ctx)
    {
        return (static_cast<word_t>(ctx.memory[ctx.pc + 2]) << 8) | ctx.memory[ctx.pc + 1];
    }
};

struct stack_ptr
{
    [[nodiscard]]
    static
    constexpr
    word_t get(cpu& ctx)
    {
        return ctx.sp;
    }

    static
    constexpr
    void set(cpu& ctx, word_t value)
    {
        ctx.sp = value;
    }
};

struct no_action
{
    static
    constexpr
    void exec(cpu& ctx) {}
};

template<class A, class B>
struct exchange
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        auto tmp{A::get(ctx)};
        A::set(ctx, B::get(ctx));
        B::set(ctx, tmp);
    }
};

struct program_counter
{
    [[nodiscard]]
    static
    constexpr
    word_t get(cpu& ctx)
    {
        return ctx.pc;
    }

    static
    constexpr
    void set(cpu& ctx, word_t value)
    {
        ctx.pc = value;
    }
};

struct reg_psw
{
    [[nodiscard]]
    static
    constexpr
    word_t get(cpu& ctx)
    {
        return (static_cast<word_t>(reg_a::get(ctx)) << 8) | flags_bits::get(ctx);
    }

    static
    constexpr
    void set(cpu& ctx, word_t value)
    {
        reg_a::set(ctx, value >> 8);
        flags_bits::set(ctx, value & 0xff);
    }
};

struct unimplemented
{
    static
    void exec(cpu& ctx)
    {
        throw unimplemented_instruction_exception{};
    }
};

template<class Condition, class ActionIf, class ActionElse, bool EqualTo = true>
struct if_true_else
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        if(Condition::is_true(ctx) == EqualTo) {
            ActionIf::exec(ctx);
        } else {
            ActionElse::exec(ctx);
        }
    }
};

template<class Condition>
struct negate
{
    [[nodiscard]]
    static
    constexpr
    bool is_true(cpu& ctx)
    {
        return !Condition::is_true(ctx);
    }
};

struct flag_is_z
{
    [[nodiscard]]
    static
    constexpr
    bool is_true(cpu& ctx)
    {
        return flag_z::get(ctx) == 1;
    }
};

using flag_is_nz = negate<flag_is_z>;

struct flag_has_c
{
    [[nodiscard]]
    static
    constexpr
    bool is_true(cpu& ctx)
    {
        return flag_c::get(ctx) == 1;
    }
};

using flag_has_nc = negate<flag_has_c>;

struct flag_has_pe
{
    [[nodiscard]]
    static
    constexpr
    bool is_true(cpu& ctx)
    {
        return flag_p::get(ctx) == PARITY_EVEN;
    }
};

using flag_has_po = negate<flag_has_pe>;

struct flag_has_s
{
    [[nodiscard]]
    static
    constexpr
    bool is_true(cpu& ctx)
    {
        return flag_s::get(ctx) == 1;
    }
};

using flag_has_ns = negate<flag_has_s>;

struct rotate_acc_left_nc
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        auto prev_c = flag_c::get(ctx);
        flag_c::set(ctx, reg_a::get(ctx) >> 7);
        reg_a::set(ctx, (reg_a::get(ctx) << 1) | prev_c);
    }
};

struct rotate_acc_left_c
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        flag_c::set(ctx, reg_a::get(ctx) >> 7);
        reg_a::set(ctx, (reg_a::get(ctx) << 1) | flag_c::get(ctx));
    }
};

struct rotate_acc_right_nc
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        auto prev_c = flag_c::get(ctx);
        flag_c::set(ctx, reg_a::get(ctx) & 1);
        reg_a::set(ctx, (reg_a::get(ctx) >> 1) | (prev_c << 7));
    }
};

struct rotate_acc_right_c
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        flag_c::set(ctx, reg_a::get(ctx) & 1);
        reg_a::set(ctx, (reg_a::get(ctx) >> 1) | (flag_c::get(ctx) << 7));
    }
};

struct complement_carry
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        flag_c::set(ctx, 1 - flag_c::get(ctx));
    }
};

struct set_carry
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        flag_c::set(ctx, 1);
    }
};

struct complement_acc
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        reg_a::set(ctx, ~reg_a::get(ctx));
    }
};

struct enable_interrupt
{
    static
    constexpr
    void set(cpu& ctx, bool value)
    {
        ctx.int_enabled = value;
    }
};

template<class Port>
struct in_callback
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        if(ctx.in_cb.has_value()) {
            reg_a::set(ctx, ctx.in_cb.value()(Port::get(ctx)));
        }
    }
};

template<class Port>
struct out_callback
{
    static
    constexpr
    void exec(cpu& ctx)
    {
        if(ctx.out_cb.has_value()) {
            ctx.out_cb.value()(Port::get(ctx), reg_a::get(ctx));
        }
    }
};

namespace instructions
{

using nop = step<1>;
using unimpl = unimplemented;

//using set_c_flag_from_tmp    = set_flags_from_value<step_tmp_value, flag_c>;
//using set_all_flags_from_tmp = set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>;
//using set_zsp_flags_from_tmp = set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p>;

template<class... Flags>
using set_flags_from_tmp = set_flags_from_value<step_tmp_value, Flags...>;

// TODO
template<class Other, class Operator, class Flags = set_flags_from_tmp<flag_z, flag_s, flag_p, flag_c>, std::size_t Step = 1, class Dst = reg_a>
using basic_arithmetic_ass =
    chain<
        assign<Dst, apply<Dst, Other, Operator, true>>,
        Flags,
        step<Step>
    >;

template<class Other>
using basic_add = basic_arithmetic_ass<Other, plus>;

using add_b = basic_add<reg_b>;
using add_c = basic_add<reg_c>;
using add_d = basic_add<reg_d>;
using add_e = basic_add<reg_e>;
using add_h = basic_add<reg_h>;
using add_l = basic_add<reg_l>;
using add_m = basic_add<at_memory<reg_hl, byte_t>>;
using add_a = basic_add<reg_a>;

template<class Other>
using basic_sub = basic_arithmetic_ass<Other, minus>;

using sub_b = basic_sub<reg_b>;
using sub_c = basic_sub<reg_c>;
using sub_d = basic_sub<reg_d>;
using sub_e = basic_sub<reg_e>;
using sub_h = basic_sub<reg_h>;
using sub_l = basic_sub<reg_l>;
using sub_m = basic_sub<at_memory<reg_hl, byte_t>>;
using sub_a = basic_sub<reg_a>;

template<class Other>
using basic_ana = basic_arithmetic_ass<Other, bit_and>;

using ana_b = basic_ana<reg_b>;
using ana_c = basic_ana<reg_c>;
using ana_d = basic_ana<reg_d>;
using ana_e = basic_ana<reg_e>;
using ana_h = basic_ana<reg_h>;
using ana_l = basic_ana<reg_l>;
using ana_m = basic_ana<at_memory<reg_hl, byte_t>>;
using ana_a = basic_ana<reg_a>;

template<class Other>
using basic_ora = basic_arithmetic_ass<Other, bit_or>;

using ora_b = basic_ora<reg_b>;
using ora_c = basic_ora<reg_c>;
using ora_d = basic_ora<reg_d>;
using ora_e = basic_ora<reg_e>;
using ora_h = basic_ora<reg_h>;
using ora_l = basic_ora<reg_l>;
using ora_m = basic_ora<at_memory<reg_hl, byte_t>>;
using ora_a = basic_ora<reg_a>;

template<class Other>
using basic_xra = basic_arithmetic_ass<Other, bit_xor>;

using xra_b = basic_xra<reg_b>;
using xra_c = basic_xra<reg_c>;
using xra_d = basic_xra<reg_d>;
using xra_e = basic_xra<reg_e>;
using xra_h = basic_xra<reg_h>;
using xra_l = basic_xra<reg_l>;
using xra_m = basic_xra<at_memory<reg_hl, byte_t>>;
using xra_a = basic_xra<reg_a>;

template<class Reg, class Operator, class Flags = set_flags_from_tmp<flag_z, flag_s, flag_p>>
using basic_by_one =
    chain<
        assign<Reg, apply<Reg, static_value<word_t, 1>, Operator, true>>,
        Flags,
        step<1>
    >;

template<class Reg>
using basic_inr = basic_by_one<Reg, plus>;

using inr_b = basic_inr<reg_b>;
using inr_c = basic_inr<reg_c>;
using inr_d = basic_inr<reg_d>;
using inr_e = basic_inr<reg_e>;
using inr_h = basic_inr<reg_h>;
using inr_l = basic_inr<reg_l>;
using inr_m = basic_inr<at_memory<reg_hl, byte_t>>;
using inr_a = basic_inr<reg_a>;

template<class Reg>
using basic_dcr = basic_by_one<Reg, minus>;

using dcr_b = basic_dcr<reg_b>;
using dcr_c = basic_dcr<reg_c>;
using dcr_d = basic_dcr<reg_d>;
using dcr_e = basic_dcr<reg_e>;
using dcr_h = basic_dcr<reg_h>;
using dcr_l = basic_dcr<reg_l>;
using dcr_m = basic_dcr<at_memory<reg_hl, byte_t>>;
using dcr_a = basic_dcr<reg_a>;

template<class Other, class Operator, std::size_t Step = 1>
using basic_arithmetic_no_ass =
    chain<
        apply<reg_a, Other, Operator, true>,
        set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>,
        step<Step>
    >;

template<class Reg>
using basic_cmp = basic_arithmetic_no_ass<Reg, minus>;

using cmp_b = basic_cmp<reg_b>;
using cmp_c = basic_cmp<reg_c>;
using cmp_d = basic_cmp<reg_d>;
using cmp_e = basic_cmp<reg_e>;
using cmp_h = basic_cmp<reg_h>;
using cmp_l = basic_cmp<reg_l>;
using cmp_m = basic_cmp<at_memory<reg_hl, byte_t>>;
using cmp_a = basic_cmp<reg_a>;

template<class Operator>
using basic_arithmetic_data_ass = basic_arithmetic_ass<following_data<byte_t>, Operator, set_flags_from_tmp<flag_z, flag_s, flag_p, flag_c>, 2>;
    //chain<
    //    assign<reg_a, apply<reg_a, following_data<byte_t>, Operator, true>>,
    //    set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>,
    //    step<2>
    //>;

using adi = basic_arithmetic_data_ass<plus>;
using sui = basic_arithmetic_data_ass<minus>;
using ani = basic_arithmetic_data_ass<bit_and>;
using ori = basic_arithmetic_data_ass<bit_or>;
using xri = basic_arithmetic_data_ass<bit_xor>;

template<class Operator>
using basic_arithmetic_data_no_ass = basic_arithmetic_no_ass<following_data<byte_t>, Operator, 2>;
    //chain<
    //    apply<reg_a, following_data<byte_t>, Operator, true>,
    //    set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>,
    //    step<2>
    //>;

using cpi = basic_arithmetic_data_no_ass<minus>;

template<class Other, class Operator>
using basic_arithmetic_with_carry_ass =
    chain<
        assign<reg_a, apply<apply<reg_a, Other, Operator>, flag_c, Operator>, true>,
        set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>,
        step<1>
    >;

template<class Other>
using basic_adc = basic_arithmetic_with_carry_ass<Other, plus>;

using adc_b = basic_adc<reg_b>;
using adc_c = basic_adc<reg_c>;
using adc_d = basic_adc<reg_d>;
using adc_e = basic_adc<reg_e>;
using adc_h = basic_adc<reg_h>;
using adc_l = basic_adc<reg_l>;
using adc_m = basic_adc<at_memory<reg_hl, byte_t>>;
using adc_a = basic_adc<reg_a>;

template<class Other>
using basic_sbb = basic_arithmetic_with_carry_ass<Other, minus>;

using sbb_b = basic_sbb<reg_b>;
using sbb_c = basic_sbb<reg_c>;
using sbb_d = basic_sbb<reg_d>;
using sbb_e = basic_sbb<reg_e>;
using sbb_h = basic_sbb<reg_h>;
using sbb_l = basic_sbb<reg_l>;
using sbb_m = basic_sbb<at_memory<reg_hl, byte_t>>;
using sbb_a = basic_sbb<reg_a>;

template<class Operator>
using basic_arithmetic_data_with_carry_ass =
    chain<
        assign<reg_a, apply<apply<reg_a, following_data<byte_t>, Operator>, flag_c, Operator>, true>,
        set_flags_from_value<step_tmp_value, flag_z, flag_s, flag_p, flag_c>,
        step<2>
    >;

using aci = basic_arithmetic_data_with_carry_ass<plus>;
using sbi = basic_arithmetic_data_with_carry_ass<minus>;

template<class Other>
using basic_dad = basic_arithmetic_ass<Other, plus, set_flags_from_tmp<flag_c>, 1, reg_hl>;

using dad_b = basic_dad<reg_bc>;
using dad_d = basic_dad<reg_de>;
using dad_h = basic_dad<reg_hl>;
using dad_s = basic_dad<stack_ptr>;

template<class Reg>
using basic_inx = basic_by_one<Reg, plus, no_action>;

using inx_b = basic_inx<reg_bc>;
using inx_d = basic_inx<reg_de>;
using inx_h = basic_inx<reg_hl>;
using inx_s = basic_inx<stack_ptr>;

template<class Reg>
using basic_dcx = basic_by_one<Reg, minus, no_action>;

using dcx_b = basic_dcx<reg_bc>;
using dcx_d = basic_dcx<reg_de>;
using dcx_h = basic_dcx<reg_hl>;
using dcx_s = basic_dcx<stack_ptr>;

template<class Dst, class Src, std::size_t Step = 1>
using basic_mov =
    chain<
        assign<Dst, Src>,
        step<Step>
    >;

using mov_bb = basic_mov<reg_b, reg_b>;
using mov_bc = basic_mov<reg_b, reg_c>;
using mov_bd = basic_mov<reg_b, reg_d>;
using mov_be = basic_mov<reg_b, reg_e>;
using mov_bh = basic_mov<reg_b, reg_h>;
using mov_bl = basic_mov<reg_b, reg_l>;
using mov_bm = basic_mov<reg_b, at_memory<reg_hl, byte_t>>;
using mov_ba = basic_mov<reg_b, reg_a>;

using mov_cb = basic_mov<reg_c, reg_b>;
using mov_cc = basic_mov<reg_c, reg_c>;
using mov_cd = basic_mov<reg_c, reg_d>;
using mov_ce = basic_mov<reg_c, reg_e>;
using mov_ch = basic_mov<reg_c, reg_h>;
using mov_cl = basic_mov<reg_c, reg_l>;
using mov_cm = basic_mov<reg_c, at_memory<reg_hl, byte_t>>;
using mov_ca = basic_mov<reg_c, reg_a>;

using mov_db = basic_mov<reg_d, reg_b>;
using mov_dc = basic_mov<reg_d, reg_c>;
using mov_dd = basic_mov<reg_d, reg_d>;
using mov_de = basic_mov<reg_d, reg_e>;
using mov_dh = basic_mov<reg_d, reg_h>;
using mov_dl = basic_mov<reg_d, reg_l>;
using mov_dm = basic_mov<reg_d, at_memory<reg_hl, byte_t>>;
using mov_da = basic_mov<reg_d, reg_a>;

using mov_eb = basic_mov<reg_e, reg_b>;
using mov_ec = basic_mov<reg_e, reg_c>;
using mov_ed = basic_mov<reg_e, reg_d>;
using mov_ee = basic_mov<reg_e, reg_e>;
using mov_eh = basic_mov<reg_e, reg_h>;
using mov_el = basic_mov<reg_e, reg_l>;
using mov_em = basic_mov<reg_e, at_memory<reg_hl, byte_t>>;
using mov_ea = basic_mov<reg_e, reg_a>;

using mov_hb = basic_mov<reg_h, reg_b>;
using mov_hc = basic_mov<reg_h, reg_c>;
using mov_hd = basic_mov<reg_h, reg_d>;
using mov_he = basic_mov<reg_h, reg_e>;
using mov_hh = basic_mov<reg_h, reg_h>;
using mov_hl = basic_mov<reg_h, reg_l>;
using mov_hm = basic_mov<reg_h, at_memory<reg_hl, byte_t>>;
using mov_ha = basic_mov<reg_h, reg_a>;

using mov_lb = basic_mov<reg_l, reg_b>;
using mov_lc = basic_mov<reg_l, reg_c>;
using mov_ld = basic_mov<reg_l, reg_d>;
using mov_le = basic_mov<reg_l, reg_e>;
using mov_lh = basic_mov<reg_l, reg_h>;
using mov_ll = basic_mov<reg_l, reg_l>;
using mov_lm = basic_mov<reg_l, at_memory<reg_hl, byte_t>>;
using mov_la = basic_mov<reg_l, reg_a>;

using mov_mb = basic_mov<at_memory<reg_hl, byte_t>, reg_b>;
using mov_mc = basic_mov<at_memory<reg_hl, byte_t>, reg_c>;
using mov_md = basic_mov<at_memory<reg_hl, byte_t>, reg_d>;
using mov_me = basic_mov<at_memory<reg_hl, byte_t>, reg_e>;
using mov_mh = basic_mov<at_memory<reg_hl, byte_t>, reg_h>;
using mov_ml = basic_mov<at_memory<reg_hl, byte_t>, reg_l>;
using mov_mm = basic_mov<at_memory<reg_hl, byte_t>, at_memory<reg_hl, byte_t>>;
using mov_ma = basic_mov<at_memory<reg_hl, byte_t>, reg_a>;

using mov_ab = basic_mov<reg_a, reg_b>;
using mov_ac = basic_mov<reg_a, reg_c>;
using mov_ad = basic_mov<reg_a, reg_d>;
using mov_ae = basic_mov<reg_a, reg_e>;
using mov_ah = basic_mov<reg_a, reg_h>;
using mov_al = basic_mov<reg_a, reg_l>;
using mov_am = basic_mov<reg_a, at_memory<reg_hl, byte_t>>;
using mov_aa = basic_mov<reg_a, reg_a>;

template<class Dst>
using basic_mvi = basic_mov<Dst, following_data<byte_t>, 2>;

using mvi_b = basic_mvi<reg_b>;
using mvi_c = basic_mvi<reg_c>;
using mvi_d = basic_mvi<reg_d>;
using mvi_e = basic_mvi<reg_e>;
using mvi_h = basic_mvi<reg_h>;
using mvi_l = basic_mvi<reg_l>;
using mvi_m = basic_mvi<at_memory<reg_hl, byte_t>>;
using mvi_a = basic_mvi<reg_a>;

using lda = basic_mov<reg_a, at_memory<following_data<word_t>, byte_t>, 3>;

template<class Src>
using basic_ldax = basic_mov<reg_a, at_memory<Src, byte_t>>;

using ldax_b = basic_ldax<reg_bc>;
using ldax_d = basic_ldax<reg_de>;

using sta = basic_mov<at_memory<following_data<word_t>, byte_t>, reg_a , 3>;

template<class Dst>
using basic_stax = basic_mov<at_memory<Dst, byte_t>, reg_a>;

using stax_b = basic_stax<reg_bc>;
using stax_d = basic_stax<reg_de>;

using lhld = basic_mov<reg_hl, at_memory<following_data<word_t>, word_t>, 3>;
using shld = basic_mov<at_memory<following_data<word_t>, word_t>, reg_hl, 3>;

template<class Reg>
using basic_lxi = basic_mov<Reg, following_data<word_t>, 3>;

using lxi_b = basic_lxi<reg_bc>;
using lxi_d = basic_lxi<reg_de>;
using lxi_h = basic_lxi<reg_hl>;
using lxi_s = basic_lxi<stack_ptr>;

template<class A, class B, std::size_t Step = 1>
using basic_exchange =
    chain<
        exchange<A, B>,
        step<Step>
    >;

using xthl = basic_exchange<reg_hl, at_memory<stack_ptr, word_t>>;
using sphl = basic_mov<stack_ptr, reg_hl>;
using pchl = basic_mov<program_counter, reg_hl, 0>;
using xchg = basic_exchange<reg_hl, reg_de>;

template<class Src>
using basic_push =
    chain<
        assign<at_memory<apply<stack_ptr, static_value<word_t, 2>, minus>, word_t>, Src>,
        assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, minus>>,
        step<1>
    >;

using push_b = basic_push<reg_bc>;
using push_d = basic_push<reg_de>;
using push_h = basic_push<reg_hl>;
using push_psw = basic_push<reg_psw>;

template<class Dst>
using basic_pop =
    chain<
        assign<Dst, at_memory<stack_ptr, word_t>>,
        assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, plus>>,
        step<1>
    >;

using pop_b = basic_pop<reg_bc>;
using pop_d = basic_pop<reg_de>;
using pop_h = basic_pop<reg_hl>;
using pop_psw = basic_pop<reg_psw>;

using jmp = assign<program_counter, following_data<word_t>>;

using call =
    chain<
        assign<at_memory<apply<stack_ptr, static_value<word_t, 2>, minus>, word_t>, apply<program_counter, static_value<word_t, 3>, plus>>,
        assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, minus>>,
        assign<program_counter, following_data<word_t>>
    >;

using ret =
    chain<
        assign<program_counter, at_memory<stack_ptr, word_t>>,
        assign<stack_ptr, apply<stack_ptr, static_value<word_t, 2>, plus>>
    >;

template<class Condition, class Action, std::size_t Step = 3>
using basic_jcr = if_true_else<Condition, Action, step<Step>>;

template<class Condition>
using basic_jmp_con = basic_jcr<Condition, jmp>;

using jnz = basic_jmp_con<flag_is_nz>;
using jz  = basic_jmp_con<flag_is_z>;
using jnc = basic_jmp_con<flag_has_nc>;
using jc  = basic_jmp_con<flag_has_c>;
using jpo = basic_jmp_con<flag_has_po>;
using jpe = basic_jmp_con<flag_has_pe>;
using jp  = basic_jmp_con<flag_has_ns>;
using jm  = basic_jmp_con<flag_has_s>;

template<class Condition>
using basic_call_con = basic_jcr<Condition, call>;

using cnz = basic_call_con<flag_is_nz>;
using cz  = basic_call_con<flag_is_z>;
using cnc = basic_call_con<flag_has_nc>;
using cc  = basic_call_con<flag_has_c>;
using cpo = basic_call_con<flag_has_po>;
using cpe = basic_call_con<flag_has_pe>;
using cp  = basic_call_con<flag_has_ns>;
using cm  = basic_call_con<flag_has_s>;

template<class Condition>
using basic_ret_con = basic_jcr<Condition, ret, 1>;

using rnz = basic_ret_con<flag_is_nz>;
using rz  = basic_ret_con<flag_is_z>;
using rnc = basic_ret_con<flag_has_nc>;
using rc  = basic_ret_con<flag_has_c>;
using rpo = basic_ret_con<flag_has_po>;
using rpe = basic_ret_con<flag_has_pe>;
using rp  = basic_ret_con<flag_has_ns>;
using rm  = basic_ret_con<flag_has_s>;

template<class Action, std::size_t Step = 1>
using action_then_step =
    chain<
        Action,
        step<Step>
    >;

using ral = action_then_step<rotate_acc_left_nc>;
using rlc = action_then_step<rotate_acc_left_c>;
using rar = action_then_step<rotate_acc_right_nc>;
using rrc = action_then_step<rotate_acc_right_c>;

using cmc = action_then_step<complement_carry>;
using cma = action_then_step<complement_acc>;
using stc = action_then_step<set_carry>;

using ei = action_then_step<assign<enable_interrupt, static_value<bool, true>>>;
using di = action_then_step<assign<enable_interrupt, static_value<bool, false>>>;

template<template <typename T> typename CB>
using basic_callback =
    chain<CB<following_data<byte_t>>, step<2>>;

using in  = basic_callback<in_callback>;
using out = basic_callback<out_callback>;

#define I(T) T::exec

std::array<std::function<void(cpu&)>, 0x100> table

{
#include "atat/opcodes.txt"

};

#undef I

} // namespace instructions

void
cpu::step()
{
    uint8_t op = memory[pc];

    instructions::table[op](*this);
}

} // namespace atat