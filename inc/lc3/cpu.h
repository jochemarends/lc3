#ifndef CPU_H
#define CPU_H

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


namespace lc3 {
    class cpu {
    public:
        void load(const std::ranges::input_range auto& bin) {
            m_program.clear();
            std::ranges::copy(bin, std::back_inserter(m_program));
        }

        void execute(std::uint16_t bin);

        void run();
    private:
        template<opcode Opcode>
        void perform(std::uint16_t bin);

        void setcc(std::int16_t value);

        std::vector<std::uint16_t> m_program{};

        std::uint16_t m_pc{};

        std::int16_t m_regs[8]{};

        std::int16_t m_memory[100]{};

        struct {
            unsigned int n : 1;
            unsigned int z : 1;
            unsigned int p : 1;
        } m_condition{0, 1, 0};
    };

    template<>
    void cpu::perform<opcode::ADD>(std::uint16_t bin) {
        if (bit_at<5>(bin)) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] + m_regs[c];
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] + sign_extend<imm5>(c);
        }
    }

    template<>
    void cpu::perform<opcode::AND>(std::uint16_t bin) {
        if (bit_at<5>(bin)) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] + m_regs[c];
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] & sign_extend<imm5>(c);
        }
    }

    template<>
    void cpu::perform<opcode::BR>(std::uint16_t bin) {
        if (bit_at<11>(bin) && m_condition.n) {
            auto [offset] = decode<PCoffset9>(bin);
            m_pc += sign_extend<PCoffset9>(offset);
        }

        if (bit_at<10>(bin) && m_condition.z) {
            auto [offset] = decode<PCoffset9>(bin);
            m_pc += sign_extend<PCoffset9>(offset);
        }

        if (bit_at<9>(bin) && m_condition.p) {
            auto [offset] = decode<PCoffset9>(bin);
            m_pc += sign_extend<PCoffset9>(offset);
        }
    }

    template<>
    void cpu::perform<opcode::JMP>(std::uint16_t bin) { 
        m_pc = m_regs[decode<BaseR>(bin).front()];
    }

    template<>
    void cpu::perform<opcode::JSR>(std::uint16_t bin) {
        m_regs[7] = m_pc;

        if (bit_at<11>(bin) == 0) {
            auto [reg] = decode<BaseR>(bin);
            m_pc = m_regs[reg];
        }
        else {
            auto [offset] = decode<PCoffset11>(bin);
            m_pc += sign_extend<PCoffset11>(offset);
        }
    }

    template<>
    void cpu::perform<opcode::LD>(std::uint16_t bin) {
        auto [reg, offset] = decode<DR, PCoffset9>(bin);
        m_regs[reg] = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        setcc(m_regs[reg]);
    }

    template<>
    void cpu::perform<opcode::LDI>(std::uint16_t bin) {
        auto [reg, offset] = decode<DR, PCoffset9>(bin);
        auto value = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        m_regs[reg] = value;
        setcc(value);
    }

    template<>
    void cpu::perform<opcode::LDR>(std::uint16_t bin) {
        auto [dest, base, offset] = decode<DR, BaseR, offset6>(bin);
        m_regs[dest] = m_memory[base + sign_extend<offset6>(bin)];
        setcc(m_regs[dest]);
    }

    template<>
    void cpu::perform<opcode::LEA>(std::uint16_t bin) {
        auto [dest, offset] = decode<DR, PCoffset9>(bin);
        m_regs[dest] = m_pc + sign_extend<PCoffset9>(offset);
        setcc(m_regs[dest]);
    }

    template<>
    void cpu::perform<opcode::NOT>(std::uint16_t bin) {
        auto [dst, src] = decode<DR, SR1>(bin);
        m_regs[dst] = ~m_regs[src];
        setcc(m_regs[dst]);
    }

    template<>
    void cpu::perform<opcode::RTI>([[maybe_unused]] std::uint16_t bin) {
        // TODO
    }

    template<>
    void cpu::perform<opcode::ST>(std::uint16_t bin) {
        auto [src, offset] = decode<SR, PCoffset9>(bin);
        m_memory[m_pc + sign_extend<PCoffset9>(offset)] = m_regs[src];
    }

    template<>
    void cpu::perform<opcode::STI>(std::uint16_t bin) {
        auto [src, offset] = decode<SR, PCoffset9>(bin);
        auto value = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        m_memory[value] = m_regs[src];
    }

    template<>
    void cpu::perform<opcode::STR>(std::uint16_t bin) {
        auto [src, base, offset] = decode<SR, BaseR, offset6>(bin);
        m_memory[base + sign_extend<offset6>(offset)] = m_regs[src];
    }

    template<>
    void cpu::perform<opcode::TRAP>([[maybe_unused]] std::uint16_t bin) {
        // TODO
    }
}

#endif

