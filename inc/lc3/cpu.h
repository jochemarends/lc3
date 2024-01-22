#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <array>
#include <lc3/types.h>
#include <lc3/opcodes.h>
#include <lc3/extend.h>
#include <lc3/encoding.h>
#include <lc3/memory.h>
#include <fstream>


namespace lc3 {
    using word = std::uint16_t;
    using sword = std::int16_t;
}

namespace lc3 {
    class cpu {
    public:
        void load(const std::ranges::input_range auto& bin, std::uint16_t origin = 0x0000) {
            std::ranges::copy(bin, m_memory.data() + origin);
        }

        void execute(std::uint16_t bin);

        void run();
    private:
        template<opcode Opcode>
        void perform(std::uint16_t bin);

        void setcc(std::int16_t value);

        std::uint16_t m_pc{0x3000};

        std::int16_t m_regs[8]{};

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

