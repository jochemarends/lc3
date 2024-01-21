#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace lc3 {
    template<typename T>
    concept type = requires {
        T::width;
        T::index;
    };

    struct DR {
        static constexpr std::uint16_t width{3};
        static constexpr std::uint16_t index{9};
    };

    struct SR {
        static constexpr std::uint16_t width{3};
        static constexpr std::uint16_t index{9};
    };

    struct SR1 {
        static constexpr std::uint16_t width{3};
        static constexpr std::uint16_t index{6};
    };

    struct SR2 {
        static constexpr std::uint16_t width{3};
        static constexpr std::uint16_t index{0};
    };

    struct BaseR {
        static constexpr std::uint16_t width{3};
        static constexpr std::uint16_t index{6};
    };

    struct imm5 {
        static constexpr std::uint16_t width{5};
        static constexpr std::uint16_t index{0};
    };

    struct offset6 {
        static constexpr std::uint16_t width{6};
        static constexpr std::uint16_t index{0};
    };

    struct PCoffset9 {
        static constexpr std::uint16_t width{9};
        static constexpr std::uint16_t index{0};
    };

    struct PCoffset11 {
        static constexpr std::uint16_t width{11};
        static constexpr std::uint16_t index{0};
    };
}

#endif

