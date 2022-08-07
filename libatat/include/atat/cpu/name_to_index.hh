#pragma once

/**
 * @file name_to_index.hh
 *
 * Contains name_to_index declaration.
 */

namespace atat
{

/**
 * Used to convert a name to the coresponding array index.
 * Eg. register_name::A -> 0
 * @tparam Type Enum type
 * @tparam Name Enum value
 */
template<class Type, Type Name>
struct name_to_index;

}; // namespace atat