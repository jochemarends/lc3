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
        case opcode::add {
            std::uint16_t a = (bin >> 9) & 0x05;
            std::uint16_t b = (bin >> 6) & 0x05;
            std::uint16_t c = bin & 0x1F;
            auto [a, b, c] = encoding<opcode::add, reg, reg, bits<0, 0, 0>, reg>::decode(bin);

            if ((bin >> 5) & 1)  {
                return encoding<:add, reg, reg, bits<0, 0, 0>, reg>::decode(bin);
            } 
            else {
                return encoding<opcode::add, reg, reg, bits<1>, imm5>::decode(bin);
            }

            /* ADD */
            auto [a, b, c] = add::decode(bin);
            
            if (!(bin >> 5) & 1) {
                c = m_regs[c];
            }

            m_regs[a] = m_regs[b] + c;
        }
        }
    }
}

