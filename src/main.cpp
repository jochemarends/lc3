#include <lc3/cpu.h>
#include <bit>
#include <fstream>
#include <vector>

int main() {
    lc3::cpu cpu{};

    std::ifstream ifs{"../reverse-string.o", std::ios::binary};

    auto read = [&ifs]() {
        lc3::word word{};
        ifs.read(reinterpret_cast<char*>(&word), sizeof word);
        return std::byteswap(word);
    };

    lc3::word origin = read();

    std::vector<lc3::word> program{};
    while (ifs) {
        program.push_back(read());
    }

    cpu.load(program, origin);
    cpu.run();
}

