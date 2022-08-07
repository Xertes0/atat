#pragma once

/**
 * @file registers.hh
 *
 * Cpu registers.
 */

#include "atat/cpu/cpu.hh"
#include "atat/cpu/name_to_index.hh"
#include "atat/cpu/types.hh"

namespace atat
{

enum class register_name
{
    A, B, C, D, E, H, L,
    //BC, DE, HL,
};

template<>
struct name_to_index<register_name, register_name::A>
{ static constexpr std::size_t value{0}; };

template<>
struct name_to_index<register_name, register_name::B>
{ static constexpr std::size_t value{1}; };

template<>
struct name_to_index<register_name, register_name::C>
{ static constexpr std::size_t value{2}; };

template<>
struct name_to_index<register_name, register_name::D>
{ static constexpr std::size_t value{3}; };

template<>
struct name_to_index<register_name, register_name::E>
{ static constexpr std::size_t value{4}; };

template<>
struct name_to_index<register_name, register_name::H>
{ static constexpr std::size_t value{5}; };

template<>
struct name_to_index<register_name, register_name::L>
{ static constexpr std::size_t value{6}; };

template<register_name Name>
using register_name_to_index = name_to_index<register_name, Name>;

template<class Derived>
struct register_by_reference
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
struct register_by_index : public register_by_reference<register_by_index<N>>
{
    [[nodiscard]]
    static
    constexpr
    registr&
    get_ref(cpu& ctx)
    {
        return ctx.registers[N];
    }
};

template<register_name Name>
using register_by_name = register_by_index<register_name_to_index<Name>::value>;

template<class High, class Low>
struct register_pair
{
    [[nodiscard]]
    static
    constexpr
    word_t
    get(cpu& ctx)
    {
        return (static_cast<word_t>(High::get(ctx)) << 8) | Low::get(ctx);
    }

    static
    constexpr
    void
    set(cpu& ctx, word_t value)
    {
        High::set(ctx, value >> 8);
        Low::set(ctx, value & 0xff);
    }
};

using reg_a = register_by_name<register_name::A>;
using reg_b = register_by_name<register_name::B>;
using reg_c = register_by_name<register_name::C>;
using reg_d = register_by_name<register_name::D>;
using reg_e = register_by_name<register_name::E>;
using reg_h = register_by_name<register_name::H>;
using reg_l = register_by_name<register_name::L>;

using reg_bc = register_pair<reg_b, reg_c>;
using reg_de = register_pair<reg_d, reg_e>;
using reg_hl = register_pair<reg_h, reg_l>;

} // namespace atat