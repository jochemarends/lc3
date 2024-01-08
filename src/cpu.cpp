#include <lc3/cpu.h>
#include <lc3/instructions.h>

namespace lc3 {
    void cpu::run() {
        while (true) {
            execute(m_program[m_pc]);
        }
    }

    void cpu::execute(std::uint16_t bin) {
        auto opcode = bin >> 12;
        switch (opcode) {
        case add::opcode:
            
            ++m_pc;
        }
    }
}

