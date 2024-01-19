#include <lc3/cpu.h>
#include <lc3/extend.h>
#include <lc3/extend.h>
#include <lc3/opcodes.h>
#include <iostream>
#include <exception>

namespace lc3 {
    void cpu::execute(std::uint16_t bin) {
        auto opcode = static_cast<lc3::opcode>(bin >> 12);

        switch (opcode) {
        case opcode::ADD: {
            auto a = (bin >> 9) & 7;
            auto b = (bin >> 6) & 7;
            auto c = bin & 0x1F;
            
            if ((bin & 0x20) == 0) {
                c = m_regs[c];
            }

            m_regs[a] = m_regs[b] + c;
            break;
        }
        case opcode::AND: {
            auto a = (bin >> 9);
        }
        default:
            throw std::runtime_error{"ERROR: invalid opcode"};
        }
    }
}

int main() try {
    lc3::cpu cpu{};
}
catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}

