#include <atat/cpu/cpu.hh>
#include <atat/cpu/registers.hh>
#include <atat/disassembler.hh>
#include <atat/opcodes.hh>
#include <cstddef>
#include <cstdio>

int main()
{
    auto memory{atat::memory_with_rom(std::string_view{CPUDIAG_PATH}, 0x100)};

    // First instruction jmp 0x100
    memory[0] = atat::opcodes::jmp;
    memory[1] = 0x00;
    memory[2] = 0x01;

    // Fix stack pointer
    memory[368] = 0x07;

    // Skip DAA test
    memory[0x59c] = atat::opcodes::jmp;
    memory[0x59d] = 0xc2;
    memory[0x59e] = 0x05;

    // For debugging
    std::vector<atat::word_t> history{};

    atat::cpu cpu{memory.data()};
    while(true) {
        history.push_back(cpu.pc_);
        auto next{cpu.peek()};
        if(next == atat::opcodes::hlt) {
            break;
        } else if(next == atat::opcodes::call) {
            atat::word_t call_adr = (static_cast<atat::word_t>(cpu.peek(2)) << 8) | cpu.peek(1);
            // Print call
            if(call_adr == 5) {
                if(atat::reg_c::get(cpu) == 9) {
                    atat::word_t offset = atat::reg_de::get(cpu);
                    char *str = reinterpret_cast<char*>(&memory[offset+3]); // Skip prefix
                    while(*str != '$') {
                        std::printf("%c", *str++);
                    }
                    std::printf("\n");
                } else if(atat::reg_c::get(cpu) == 2) {
                    std::printf("Should not be called\n");
                    break;
                }
                return 0;
            } else if(call_adr == 0) {
                break;
                cpu.skip_bytes(3);
            }
        }

        cpu.step();
    }

    return 0;
}