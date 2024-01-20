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
        static constexpr std::size_t index{0};
    };

    struct offset6 {
        static constexpr std::size_t width{6};
    };

    struct pc_offset9 {
        static constexpr std::size_t width{9};
    };

    struct pc_offset11 {
        static constexpr std::size_t width{11};
    };
}

#endif

