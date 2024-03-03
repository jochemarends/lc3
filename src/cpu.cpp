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
// tag::run[]
    void cpu::run() {
        while (!m_halted) {
            execute(m_memory[m_pc++]);
        }
        m_halted = false;
    }
// end::run[]

// tag::add[]
    template<>
    void cpu::perform<opcode::ADD>(word bin) {
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
    void cpu::perform<opcode::BR>(word bin) {
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
    void cpu::perform<opcode::JSR>(word bin) {
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
    void cpu::perform<opcode::LD>(word bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        setcc(m_regs[idx]);
    }
// end::ld[]

// tag::ldi[]
    template<>
    void cpu::perform<opcode::LDI>(word bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_memory[m_memory[m_pc + sign_extend<PCoffset9>(offset)]];
        setcc(m_regs[idx]);
    }
// end::ldi[]

// tag::ldr[]
    template<>
    void cpu::perform<opcode::LDR>(word bin) {
        auto [a, b, offset] = decode<DR, BaseR, offset6>(bin);
        m_regs[a] = m_memory[m_regs[b] + sign_extend<offset6>(offset)];
        setcc(m_regs[a]);
    }
// end::ldr[]

// tag::lea[]
    template<>
    void cpu::perform<opcode::LEA>(word bin) {
        auto [idx, offset] = decode<DR, PCoffset9>(bin);
        m_regs[idx] = m_pc + sign_extend<PCoffset9>(offset);
        setcc(m_regs[idx]);
    }
// end::lea[]

// tag::not[]
    template<>
    void cpu::perform<opcode::NOT>(word bin) {
        auto [a, b] = decode<DR, SR1>(bin);
        m_regs[a] = ~m_regs[b];
        setcc(m_regs[a]);
    }
// end::not[]

    template<>
    void cpu::perform<opcode::RTI>([[maybe_unused]] word bin) {
        // this operation is not implemented since it requires the 
        // system call routines to be stored in memory, which is 
        // not the case.
        throw std::logic_error{"ERROR: the RTI operation is not implemented"};
    }

// tag::st[]
    template<>
    void cpu::perform<opcode::ST>(word bin) {
        auto [reg, offset] = decode<SR, PCoffset9>(bin);
        auto address = m_pc + sign_extend<PCoffset9>(offset);
        m_memory[address] = m_regs[reg];
    }
// end::st[]

// tag::sti[]
    template<>
    void cpu::perform<opcode::STI>(word bin) {
        auto [reg, offset] = decode<SR, PCoffset9>(bin);
        auto address = m_memory[m_pc + sign_extend<PCoffset9>(offset)];
        m_memory[address] = m_regs[reg];
    }
// end::sti[]

// tag::str[]
    template<>
    void cpu::perform<opcode::STR>(word bin) {
        auto [a, b, offset] = decode<SR, BaseR, offset6>(bin);
        m_memory[m_regs[b] + sign_extend<offset6>(offset)] = m_regs[a];
    }
// end::str[]

// tag::trap[]
    template<>
    void cpu::perform<opcode::TRAP>(word bin) {
        enum class vectors {
            GETC  = 0x20,
            OUT   = 0x21,
            PUTS  = 0x22,
            IN    = 0x23,
            PUTSP = 0x24,
            HALT  = 0x25,
        };

        using enum vectors;

        auto [trapvect] = decode<trapvect8>(bin);
        auto vector = static_cast<vectors>(trapvect);

        // character input
        if (vector == GETC || vector == IN) {
            if (vector == IN) {
                std::cout << "Enter a character.\n";
            }
            m_regs[0] = std::cin.get();
        }

        // character output
        if (vector == OUT) {
            std::cout << static_cast<char>(m_regs[0]);
        }

        // string output
        if (vector == PUTS) {
            for (auto address = m_regs[0]; m_memory[address] != 0; ++address) {
                auto ch = static_cast<char>(m_memory[address]);
                std::cout << ch;
            }
        }

        // halting the CPU
        if (vector == HALT) {
            m_halted = true;
        }

        if (vector == PUTSP) {
            throw std::logic_error{"ERROR: PUTSP is not implemented"};
        }
    }
// end::trap[]

// tag::execute[]
    void cpu::execute(word bin) {
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
// end::execute[]
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

// tag::setcc[]
    void cpu::setcc(std::int16_t value) {
        m_condition.n = (value < 0);
        m_condition.z = (value == 0);
        m_condition.p = (value > 0);
    }
// end::setcc[]
}

