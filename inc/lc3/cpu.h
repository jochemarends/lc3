#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <algorithm>
#include <ranges>

namespace lc3 {
    class cpu {
    public:
        void load(const std::input_range auto& bin) {
            m_program.clear();
            std::ranges::copy(bin, std::back_insterter(m_program));
        }

        void execute(std::uint16_t bin);

        void run();
    private:
        std::vector<std::uint16_t> m_program{};

        std::uint16_t m_pc{};

        std::int16_t m_regs[8]{};

        struct {
            unsigned int n : 1;
            unsigned int z : 1;
            unsigned int p : 1;
        } m_condition{};
    };
}

#endif

