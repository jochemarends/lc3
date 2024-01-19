#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace lc3 {
    template<typename T>
    concept type = requires {
        T::width;
    };

    struct reg {
        static constexpr std::size_t width{3};
    };

    struct imm5 {
        static constexpr std::size_t width{5};
    };
}

#endif

