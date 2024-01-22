#include <lc3/cpu.h>
#include <lc3/extend.h>
#include <lc3/extend.h>
#include <lc3/opcodes.h>
#include <iostream>
#include <exception>

namespace lc3 {
    void cpu::run() {
        while(!m_halted) {
            execute(m_memory[m_pc++]);
        }
    }

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
    [[maybe_unused]] std::uint16_t program[]{
        0xE002,
        0xF022,
        0xF025,
        0x0048,
        0x0000
    };

    [[maybe_unused]] std::uint16_t prog[]{
        0x200A,
        0x260B,
        0x2409,
        0x1E20,
        0xE009,
        0xF022,
        0x11E0,
        0x16C0,
        0x14BF,
        0x0BF9,
        0xF025,
        0x0003,
        0x000A,
        0x0000,
        0x0068,
        0x006F,
        0x0069,
        0x0000,
        0x0000,
    };

    lc3::cpu cpu{};
    cpu.load_from_obj_file("./wheel");
    cpu.run();
}

