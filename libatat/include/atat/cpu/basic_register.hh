#pragma once

/**
 * @file basic_register.hh
 *
 * Contains register definintion.
 */

#include "atat/cpu/types.hh"

namespace atat
{

/**
 * Describes a single register.
 * register is a reserved keyword thus the spelling.
 */
struct registr
{
    byte_t value;

    constexpr
    auto&
    operator=(byte_t other)
    {
        value = other;
        return *this;
    }
};

} // namespace atat