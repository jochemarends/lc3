#include <lc3/cpu.h>
#include <lc3/extend.h>
#include <lc3/extend.h>
#include <lc3/opcodes.h>
#include <iostream>
#include <exception>

namespace lc3 {
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
            throw std::runtime_error{"ERROR: invalid opcode"};
        }
    }

    void cpu::setcc(std::int16_t value) {
        m_condition.n = (value > 0);
        m_condition.z = (value == 0);
        m_condition.p = (value < 0);
    }
}

int main() {
    lc3::cpu cpu{};
}

