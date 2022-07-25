#include "atat/cpu/flags.hh"

#include <limits>

#define Z() \
    z = (val == 0)?1:0;
#define S() \
    s = ((val & 0b1000000) == 1)?1:0;
#define C() \
    c = (val > std::numeric_limits<uint8_t>::max())?1:0;
#define P() \
    uint8_t count{0}; \
    for(uint8_t i=0;i<8;i++) { \
        count += ((val & (1 << i)) > 0)?1:0; \
    } \
    p = (count%2 == 0)?1:0;

namespace atat
{

flags::flags() :
    z{0},
    s{0},
    p{0},
    c{0}/*,
    a{0}*/ {}

void
flags::set_zspc(uint16_t val)
{
    Z();
    S();
    P();
    C();
}

void
flags::set_zsp(uint16_t val)
{
    Z();
    S();
    P();
}

void
flags::set_c(uint16_t val)
{
    C();
}

} // namespace atat