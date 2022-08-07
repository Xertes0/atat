#pragma once

/**
 * @file flags.hh
 *
 * Cpu flags.
 */

#include "atat/cpu/cpu.hh"
#include "atat/cpu/name_to_index.hh"
#include "atat/cpu/types.hh"

namespace atat
{

static constexpr byte_t PARITY_EVEN{1};
static constexpr byte_t PARITY_ODD {0};

enum class flag_name
{
    Z, S, P, C,
};

template<>
struct name_to_index<flag_name, flag_name::Z>
{ static constexpr std::size_t value{0}; };

template<>
struct name_to_index<flag_name, flag_name::S>
{ static constexpr std::size_t value{1}; };

template<>
struct name_to_index<flag_name, flag_name::P>
{ static constexpr std::size_t value{2}; };

template<>
struct name_to_index<flag_name, flag_name::C>
{ static constexpr std::size_t value{3}; };

template<flag_name Name>
using flag_name_to_index = name_to_index<flag_name, Name>;

template<class Derived>
struct flag_by_reference
{
    static
    constexpr
    void
    set(cpu& ctx, uint8_t val)
    {
        Derived::get_ref(ctx) = val;
    }

    [[nodiscard]]
    static
    constexpr
    uint8_t
    get(cpu& ctx)
    {
        return Derived::get_ref(ctx).value;
    }
};

template<std::size_t N>
struct flag_by_index : public flag_by_reference<flag_by_index<N>>
{
    [[nodiscard]]
    static
    constexpr
    flag&
    get_ref(cpu& ctx)
    {
        return ctx.flags[N];
    }
};

template<flag_name Name>
using flag_by_name = flag_by_index<flag_name_to_index<Name>::value>;

/**
 * Zero flag.
 * Set to 1 if result is zero.
 */
using flag_z = flag_by_name<flag_name::Z>;

/**
 * Sign flag.
 * Set to 1 if the sign bit is set.
 */
using flag_s = flag_by_name<flag_name::S>;

/**
 * Parity flag.
 * See PARITY_EVEN and PARITY_ODD constants.
 */
using flag_p = flag_by_name<flag_name::P>;

/**
 * Carry flag.
 * Set to 1 when overflow.
 */
using flag_c = flag_by_name<flag_name::C>;

/**
 * Simulate flag register.
 * Flag register bits: SZ0A0P1C.
 */
struct flags_bits
{
    [[nodiscard]]
    static
    constexpr
    byte_t get(cpu& ctx)
    {
        return
            flag_s::get(ctx) << 7 |
            flag_z::get(ctx) << 6 |
            0 << 5 |
            0 << 4 | // auxillary flag is not implemented
            0 << 3 |
            flag_p::get(ctx) << 2 |
            1 << 1 |
            flag_c::get(ctx);
    }

    static
    constexpr
    void set(cpu& ctx, byte_t value)
    {
        flag_s::set(ctx, (value >> 7) & 1);
        flag_z::set(ctx, (value >> 6) & 1);
        flag_p::set(ctx, (value >> 2) & 1);
        flag_c::set(ctx, value & 1);
    }
};

} // namespace atat