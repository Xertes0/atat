#pragma once

/**
 * @file basic_flag.hh
 *
 * Contains flag definintion.
 */

#include "atat/cpu/types.hh"

namespace atat
{

/**
 * Describes a single flag.
 */
struct flag
{
    byte_t value:1;

    constexpr
    auto&
    operator=(byte_t other)
    {
        value = other;
        return *this;
    }
};

} // namespace atat