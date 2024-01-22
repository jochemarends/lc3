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
#include <fstream>


namespace lc3 {
    class cpu {
    public:
        void load(const std::ranges::input_range auto& bin) {
            std::ranges::copy(bin, m_memory + 0x3000);
        }

        void load_from_obj_file(const std::string& file_name) {
            std::ifstream ifs{file_name, std::ios::binary};
            ifs.good();

            std::uint16_t n{};
            auto idx = 0x3000;
            while (ifs.read(reinterpret_cast<char*>(&n), sizeof n)) {
               auto temp = n >> 8 & 0xFF;
               n = n << 8;
               n = n & 0xFF00;
               n = n | temp;
               //std::cout << std::hex << n << '\n';
               m_memory[idx++] = n;
            }
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

        std::int16_t m_memory[0xFFFF]{};

        bool m_halted{};

        struct {
            unsigned int n : 1;
            unsigned int z : 1;
            unsigned int p : 1;
        } m_condition{0, 1, 0};
    };

    template<>
    void cpu::perform<opcode::ADD>(std::uint16_t bin) {
        if (bit_at<5>(bin) == 0) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] + m_regs[c];
            setcc(m_regs[a]);
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] + sign_extend<imm5>(c);
            //std::cout << "ADD: " << a << ", " << b << ", " << sign_extend<imm5>(c) << '\n';
            //char ch{};
            //std::cin >> ch;
            setcc(m_regs[a]);
        }
    }

    template<>
    void cpu::perform<opcode::AND>(std::uint16_t bin) {
        if (bit_at<5>(bin)) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = m_regs[b] & m_regs[c];
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = m_regs[b] & sign_extend<imm5>(c);
        }
    }

    template<>
    void cpu::perform<opcode::BR>(std::uint16_t bin) {
        auto [offset] = decode<PCoffset9>(bin);

        if (bit_at<11>(bin) && m_condition.n) {
            m_pc += sign_extend<PCoffset9>(offset);
        }

        if (bit_at<10>(bin) && m_condition.z) {
            m_pc += sign_extend<PCoffset9>(offset);
        }

        if (bit_at<9>(bin) && m_condition.p) {
            m_pc += sign_extend<PCoffset9>(offset);
        }
    }

    template<>
    void cpu::perform<opcode::JMP>(std::uint16_t bin) { 
        auto [idx] = decode<BaseR>(bin);
        m_pc = m_regs[idx];
    }

    template<>
    void cpu::perform<opcode::JSR>(std::uint16_t bin) {
        m_regs[7] = m_pc;

        if (bit_at<11>(bin) == 0) {
            auto [idx] = decode<BaseR>(bin);
            m_pc = m_regs[idx];
        }
        else {
            auto [offset] = decode<PCoffset11>(bin);
            m_pc += sign_extend<PCoffset11>(offset);
        }
    }

    template<>
    void cpu::perform<opcode::LD>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        setcc(m_regs[idx]);
    }

    template<>
    void cpu::perform<opcode::LDI>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        setcc(m_regs[idx]);
    }

    template<>
    void cpu::perform<opcode::LDR>(std::uint16_t bin) {
        auto [a, b, offset] = decode<DR, BaseR, offset6>(bin);
        m_regs[a] = m_memory[b + sign_extend<offset6>(offset)];
        setcc(m_regs[a]);
    }

    template<>
    void cpu::perform<opcode::LEA>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_pc + sign_extend<PCoffset9>(offset);
        setcc(m_regs[idx]);
    }

    template<>
    void cpu::perform<opcode::NOT>(std::uint16_t bin) {
        auto [a, b] = decode<DR, SR1>(bin);
        m_regs[a] = ~m_regs[b];
        setcc(m_regs[a]);
    }

    template<>
    void cpu::perform<opcode::RTI>([[maybe_unused]] std::uint16_t bin) {
        // TODO
    }

    template<>
    void cpu::perform<opcode::ST>(std::uint16_t bin) {
        auto [idx, offset] = decode<SR, PCoffset9>(bin);
        m_memory[m_pc + sign_extend<PCoffset9>(offset)] = m_regs[idx];
    }

    template<>
    void cpu::perform<opcode::STI>(std::uint16_t bin) {
        auto [a, offset] = decode<SR, PCoffset9>(bin);
        auto b = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        m_memory[b] = m_regs[a];
    }

    template<>
    void cpu::perform<opcode::STR>(std::uint16_t bin) {
        auto [a, b, offset] = decode<SR, BaseR, offset6>(bin);
        m_memory[b + sign_extend<offset6>(offset)] = m_regs[a];
    }

    template<>
    void cpu::perform<opcode::TRAP>(std::uint16_t bin) {
        auto [offset] = decode<trapvect8>(bin);
        offset = zero_extend<trapvect8>(offset);
        offset = offset & 0xFF;
        m_regs[7] = m_pc;
        // m_pc = m_memory[zero_extend<trapvect8>(offset)];

        if (offset == 0x25) {
            m_halted = true;
            return;
        }

        if (offset == 0x22) {
            std::uint16_t idx = m_regs[0];
            while (m_memory[idx] != 0x0000) {
                std::cout << static_cast<unsigned char>(m_memory[idx]);
                ++idx;
            }
            return;
        }

        if (offset == 0x20) {
            m_regs[0] = 'a';
        }
    }
}

#endif

