#pragma once

/**
 * @file cpu.hh
 *
 * Main cpu logic.
 */

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

#include "atat/cpu/basic_flag.hh"
#include "atat/cpu/basic_register.hh"
#include "atat/exceptions.hh"
#include "atat/cpu/types.hh"

namespace atat
{

static constexpr word_t ROM_SIZE{0x2000};
static constexpr word_t MEMORY_SIZE{0x4000};

using in_callback_t  = std::function<byte_t(byte_t)>;
using out_callback_t = std::function<void(byte_t,byte_t)>;

/**
 * CPU
 */
class cpu
{
public:
    /**
     * Register array.
     * See name_to_index in registers.hh for order.
     */
    std::array<registr, 7> registers
    {
        registr{},
        registr{},
        registr{},
        registr{},
        registr{},
        registr{},
        registr{},
    };

    /**
     * Flag array.
     * See name_to_index in flags.hh for order.
     */
    std::array<flag, 4> flags
    {
        flag{},
        flag{},
        flag{},
        flag{},
    };

    /**
     * Pointer to memory.
     * Layout: \n
     *  0x4000 - 0xffff -> RAM Mirror \n
     *  0x2400 - 0x3fff -> Video RAM  \n
     *  0x2000 - 0x23ff -> Work RAM   \n
     *  0x0000 - 0x1fff -> ROM        \n
     */
    byte_t* memory;

    word_t step_tmp; ///< Used in cpu_step.cc
    word_t pc; ///< Program counter
    word_t sp; ///< Stack pointer
    byte_t int_enabled; ///< Interrupts enabled

    std::optional<in_callback_t>  in_cb;
    std::optional<out_callback_t> out_cb;

    /**
     * Constructor.
     *
     * @param memory Pointer to cpu memory
     * @param in_callback (optional) IN callback, first parameter specifies the port
     * @param out_callback (optional) OUT callback, first parameter specifies the port, second is register a value
     */
    cpu(
        byte_t* memory,
        std::optional<in_callback_t>  in_callback  = std::nullopt,
        std::optional<out_callback_t> out_callback = std::nullopt
    );

    /**
     * Step cpu by a single instruction.
     */
    void
    step();

    /**
     * Return the next instruction to be executed or next count bytes.
     * Does not advance the program counter.
     *
     * @param count Amount of bytes after next instruction
     * @return byte_t Next instruction
     */
    byte_t
    peek(word_t count = 0);

    /**
     * Advances the program counter by the given value.
     */
    void
    skip_bytes(word_t count);

    /**
     * Generates an interrupt and executes RST if interrupts enabled.
     * Disables interrupts.
     * Pushes program counter on the stack and sets it to 8*num.
     * @param num Interrupt number
     */
    void
    generate_int(byte_t num);
};

} // namespace atat
