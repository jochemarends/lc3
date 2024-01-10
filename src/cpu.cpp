#include <lc3/cpu.h>
#include <lc3/instructions.h>

namespace lc3 {
    void cpu::run() {
        while (true) {
            execute(m_program[m_pc++]);
        }
    }

    void cpu::execute(std::uint16_t bin) {
        auto opcode = bin >> 12;
        switch (opcode) {
        case add::opcode: {
            auto [a, b, c] = decode<add>(bin);

            if (bit_at<5>(bin) == 0) {
                c = regs[c];
            }

            regs[a] = regs[b] + c;
        }
        }
    }
}

