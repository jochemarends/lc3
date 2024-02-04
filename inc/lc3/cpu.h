// tag::ignore[]

/**
 * @file cpu.h
 * @author Jochem Arends
 */

// end::ignore[]

#ifndef CPU_H
#define CPU_H

#include <lc3/memory.h>
#include <lc3/opcodes.h>
#include <lc3/types.h>
#include <algorithm>
#include <ranges>

namespace lc3 {
    class cpu {
    public:
        void load(const std::ranges::input_range auto& bin, word offset = 0x0000) {
            std::ranges::copy(bin, std::next(m_memory.begin(), offset));
        }

        void execute(word bin);

        void run();
    private:
        template<opcode Opcode>
        void perform(word bin);

        void setcc(sword value);

        word m_pc{0x3000};

        sword m_regs[8]{};

        memory m_memory{};

        bool m_halted{};

        struct {
            unsigned int n : 1;
            unsigned int z : 1;
            unsigned int p : 1;
        } m_condition{0, 1, 0};
    };
}

#endif

