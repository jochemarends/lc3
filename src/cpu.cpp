#include <lc3/cpu.h>
#include <lc3/extend.h>
#include <lc3/extend.h>
#include <lc3/opcodes.h>
#include <iostream>
#include <exception>

namespace lc3 {
    template<std::size_t N>
    std::uint8_t bit_at(std::integral auto bin) {
        return bin >> N & 1;
    }

    void cpu::execute(std::uint16_t bin) {
        auto opcode = static_cast<lc3::opcode>(bin >> 12);

        auto dr = [this](std::uint16_t bin) -> auto& {
            auto idx = (bin >> 9) & 7;
            return m_regs[idx];
        };

        auto sr1 = [this](std::uint16_t bin) {
            auto idx = (bin >> 6) & 7;
            return m_regs[idx];
        };

        [[maybe_unused]]
        auto sr2 = [this](std::uint16_t bin) {
            auto idx = bin & 7;
            return m_regs[idx];
        };

        auto baser = [](std::uint16_t bin) {
            return bin >> 6 & 0x3F;
        };


        switch (opcode) {
        case opcode::ADD: {
            if (bin & 0x20) {
                dr(bin) = sr1(bin) + sign_extend<imm5>(bin);
            }
            else {
                dr(bin) = sr1(bin) + sr2(bin);
            }
            break;
        }
        case opcode::AND: {
            if (bin & 0x20) {
                dr(bin) = sr1(bin) & sign_extend<imm5>(bin);
            }
            else {
                dr(bin) = sr1(bin) & sr2(bin);
            }
            break;
        }
        case opcode::BR: {
            if (((bin & 0x0400) && m_condition.n)
             || ((bin & 0x0200) && m_condition.z)
             || ((bin & 0x0100) && m_condition.p)) {              
                m_pc += sign_extend<pc_offset9>(bin);
            }
            break;
        }
        case opcode::JMP: {
            m_pc = m_regs[baser(bin)];
            break;
        }
        case opcode::JSR: {
            if (bit_at<11>(bin) == 0) {
                m_pc = m_regs[baser(bin)];
            }
            else {
                m_pc += bin & 0x0FFF;
            }
            break;
        }
        default:
            throw std::runtime_error{"ERROR: invalid opcode"};
        }
    }
}

int main() {
    lc3::cpu cpu{};
}

