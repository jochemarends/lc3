/**
 * @file cpu.cpp
 * @author Jochem Arends
 */

#include <lc3/cpu.h>
#include <lc3/extend.h>
#include <lc3/encoding.h>
#include <lc3/opcodes.h>
#include <iostream>
#include <stdexcept>

namespace lc3 {
    void cpu::run() {
        while (!m_halted) {
            std::uint16_t var = m_memory[m_pc++];
            execute(var);
        }
    }

// tag::add[]
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
            setcc(m_regs[a]);
        }
    }
// end::add[]

    template<>
    void cpu::perform<opcode::AND>(word bin) {
        if (bit_at<5>(bin) == 0) {
            auto [a, b, c] = decode<DR, SR1, SR2>(bin);
            m_regs[a] = static_cast<sword>(m_regs[b] & m_regs[c]);
            setcc(m_regs[a]);
        }
        else {
            auto [a, b, c] = decode<DR, SR1, imm5>(bin);
            m_regs[a] = static_cast<sword>(m_regs[b] & sign_extend<imm5>(c));
            setcc(m_regs[a]);
        }
    }

// tag::br[]
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
// end::br[]

// tag::jmp[]
    template<>
    void cpu::perform<opcode::JMP>(word bin) {
        auto [idx] = decode<BaseR>(bin);
        m_pc = m_regs[idx];
    }
// end::jmp[]

// tag::jsr[]
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
// end::jsr[]

// tag::ld[]
    template<>
    void cpu::perform<opcode::LD>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        setcc(m_regs[idx]);
    }
// end::ld[]

// tag::ldi[]
    template<>
    void cpu::perform<opcode::LDI>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_memory[m_pc + sign_extend<PCoffset9>(offset)]];
        setcc(m_regs[idx]);
    }
// end::ldi[]

// tag::ldr[]
    template<>
    void cpu::perform<opcode::LDR>(std::uint16_t bin) {
        auto [a, b, offset] = decode<DR, BaseR, offset6>(bin);
        m_regs[a] = m_memory[m_regs[b] + sign_extend<offset6>(offset)];
        setcc(m_regs[a]);
    }
// end::ldr[]

// tag::lea[]
    template<>
    void cpu::perform<opcode::LEA>(std::uint16_t bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_pc + sign_extend<PCoffset9>(offset);
        setcc(m_regs[idx]);
    }
// end::lea[]

// tag::not[]
    template<>
    void cpu::perform<opcode::NOT>(std::uint16_t bin) {
        auto [a, b] = decode<DR, SR1>(bin);
        m_regs[a] = ~m_regs[b];
        setcc(m_regs[a]);
    }
// end::not[]

    template<>
    void cpu::perform<opcode::RTI>([[maybe_unused]] std::uint16_t bin) {
        // this operation is not implemented since it requires the 
        // system call routines to be stored in memory, which is 
        // not the case.
        throw std::logic_error{"ERROR: the RTI operation is not implemented"};
    }

// tag::st[]
    template<>
    void cpu::perform<opcode::ST>(std::uint16_t bin) {
        auto [reg, offset] = decode<SR, PCoffset9>(bin);
        auto address = m_pc + sign_extend<PCoffset9>(offset);
        m_memory[address] = m_regs[reg];
    }
// end::st[]

// tag::sti[]
    template<>
    void cpu::perform<opcode::STI>(std::uint16_t bin) {
        auto [reg, offset] = decode<SR, PCoffset9>(bin);
        auto address = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        m_memory[address] = m_regs[reg];
    }
// end::sti[]

// tag::str[]
    template<>
    void cpu::perform<opcode::STR>(std::uint16_t bin) {
        auto [a, b, offset] = decode<SR, BaseR, offset6>(bin);
        m_memory[m_regs[b] + sign_extend<offset6>(offset)] = m_regs[a];
    }
// end::str[]

// tag::trap[]
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
            m_regs[0] = std::cin.get();
        }

        if (offset == 0x23) {
            std::cout << "enter a character\n";
            m_regs[0] = std::cin.get();
        }

        if (offset == 0x21) {
            std::cout << static_cast<char>(m_regs[0]);
        }

    }
// end::trap[]

    void cpu::execute(std::uint16_t bin) {
        auto op = static_cast<opcode>(bin >> 12);

        switch (op) {
        case opcode::ADD:
            perform<opcode::ADD>(bin);
            break;
        case opcode::AND:
            perform<opcode::AND>(bin);
            break;
        case opcode::BR:
            perform<opcode::BR>(bin);
            break;
        case opcode::JMP:
            perform<opcode::JMP>(bin);
            break;
        case opcode::JSR:
            perform<opcode::JSR>(bin);
            break;
        case opcode::LD:
            perform<opcode::LD>(bin);
            break;
        case opcode::LDI:
            perform<opcode::LDI>(bin);
            break;
        case opcode::LDR:
            perform<opcode::LDR>(bin);
            break;
        case opcode::LEA:
            perform<opcode::LEA>(bin);
            break;
        case opcode::NOT:
            perform<opcode::NOT>(bin);
            break;
        case opcode::RTI:
            perform<opcode::RTI>(bin);
            break;
        case opcode::ST:
            perform<opcode::ST>(bin);
            break;
        case opcode::STI:
            perform<opcode::STI>(bin);
            break;
        case opcode::STR:
            perform<opcode::STR>(bin);
            break;
        case opcode::TRAP:
            perform<opcode::TRAP>(bin);
            break;
        default:
            throw std::invalid_argument{"ERROR: invalid opcode"};
        }
    }

    void cpu::setcc(std::int16_t value) {
        m_condition.n = (value < 0);
        m_condition.z = (value == 0);
        m_condition.p = (value > 0);
    }
}

