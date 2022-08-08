#pragma once

/**
 * @file cpu.hh
 *
 * Main cpu logic.
 */

#include <array>
#include <cstdint>
#include <vector>

#include "atat/cpu/basic_flag.hh"
#include "atat/cpu/basic_register.hh"
#include "atat/exceptions.hh"
#include "atat/cpu/types.hh"

namespace atat
{

/// Old interface for unit tests
namespace old
{

template<std::size_t N>
struct reg
{
    void* ptr;

    reg(void* cpu) :
        ptr{cpu} {}

    operator uint8_t() const;
    reg<N>& operator=(uint8_t other);
};

struct regs
{
    void* ptr;

    old::reg<0> a;
    old::reg<1> b;
    old::reg<2> c;
    old::reg<3> d;
    old::reg<4> e;
    old::reg<5> h;
    old::reg<6> l;

    regs(void* ptr) :
        ptr{ptr},
        a{ptr},
        b{ptr},
        c{ptr},
        d{ptr},
        e{ptr},
        h{ptr},
        l{ptr} {}

    uint16_t bc() const;
    uint16_t de() const;
    uint16_t hl() const;
    void set_bc(uint16_t);
    void set_de(uint16_t);
    void set_hl(uint16_t);
};

template<std::size_t N>
struct flag
{
    void* ptr;

    flag(void* cpu) :
        ptr{cpu} {}

    operator uint8_t() const;
    flag<N>& operator=(uint8_t other);
};

struct flags
{
    void* ptr;

    old::flag<0> z;
    old::flag<1> s;
    old::flag<2> p;
    old::flag<3> c;

    flags(void* ptr) :
        ptr{ptr},
        z{ptr},
        s{ptr},
        p{ptr},
        c{ptr} {}

    uint8_t bits() const;
};

} // namespace atat::old

static constexpr word_t ROM_SIZE{0x2000};
static constexpr word_t MEMORY_SIZE{0x3fff};

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
    word_t pc_; ///< Program counter
	word_t sp_; ///< Stack pointer
	byte_t int_enabled_; ///< Interrupts enabled

	/**
	 * Constructor
	 *
	 * @param memory Pointer to cpu memory
	 */
	cpu(byte_t* memory);

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

    old::regs regs_;   ///< Old registers interface for unit tests.
    old::flags flags_; ///< Old falgs interface for unit tests.
};

namespace old
{

template<std::size_t N>
reg<N>::operator uint8_t() const
{
    return reinterpret_cast<cpu*>(ptr)->registers[N].value;
}

template<std::size_t N>
reg<N>& reg<N>::operator=(uint8_t other)
{
    reinterpret_cast<cpu*>(ptr)->registers[N].value = other;
}

template<std::size_t N>
flag<N>::operator uint8_t() const
{
    return reinterpret_cast<cpu*>(ptr)->flags[N].value;
}

template<std::size_t N>
flag<N>& flag<N>::operator=(uint8_t other)
{
    reinterpret_cast<cpu*>(ptr)->flags[N].value = other;
}

} // namespace atat::old

} // namespace atat
