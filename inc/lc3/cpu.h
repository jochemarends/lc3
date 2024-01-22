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
        void load(const std::ranges::input_range auto& bin, std::uint16_t origin = 0x0000) {
            std::ranges::copy(bin, m_memory + origin);
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

}

#endif

