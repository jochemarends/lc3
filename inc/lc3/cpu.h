#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ranges>

namespace lc3 {
    class cpu {
    public:
        void load(const std::ranges::input_range auto& bin) {
            m_program.clear();
            std::ranges::copy(bin, std::back_inserter(m_program));
        }

        void execute(std::uint16_t bin);

        template<opcode Opcode>
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
        } m_condition{0, 1, 0};
    };

    template<>
    void cpu::execute<opcode::ADD>(std::uint16_t bin) {
        if (bin & 0x20) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] + m_regs[c];
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] + sign_extend<imm5>(c);
        }
    }

    template<>
    void cpu::execute<opcode::AND>(std::uint16_t bin) {
        if (bin & 0x20) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] + m_regs[c];
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] + sign_extend<imm5>(c);
        }
    }

    template<>
    void cpu::execute<opcode::BR>(std::uint16_t bin) {
        auto offset = decode<PCOffset9>(bin);

        if (bin & 0x0400 && m_condition.n) {
            m_pc += sign_extend<PCOffset9>(bin);
        }

        if (bin & 0x0200 && m_condition.z) {
            m_pc += sign_extend<PCOffset9>(bin);
        }

        if (bin & 0x0400 && m_condition.p) {
            auto offset = decode<PCOffset9>(bin);
            m_pc += sign_extend<PCOffset9>(offset);
        }
    }
}

#endif

