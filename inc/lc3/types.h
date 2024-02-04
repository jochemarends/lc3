/**
 * @file types.h
 * @author Jochem Arends
 */

// tag::adoc[]

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace lc3 {
    using word = std::uint16_t;
    using sword = std::int16_t;

    template<typename T>
    concept type = requires {
        T::width;
        T::index;
    };

    struct DR {
        static constexpr word width{3};
        static constexpr word index{9};
    };

    struct SR {
        static constexpr word width{3};
        static constexpr word index{9};
    };

// end::adoc[]

    struct SR1 {
        static constexpr word width{3};
        static constexpr word index{6};
    };

    struct SR2 {
        static constexpr word width{3};
        static constexpr word index{0};
    };

    struct BaseR {
        static constexpr word width{3};
        static constexpr word index{6};
    };

    struct imm5 {
        static constexpr word width{5};
        static constexpr word index{0};
    };

    struct offset6 {
        static constexpr word width{6};
        static constexpr word index{0};
    };

    struct PCoffset9 {
        static constexpr word width{9};
        static constexpr word index{0};
    };

    struct PCoffset11 {
        static constexpr word width{11};
        static constexpr word index{0};
    };

    struct trapvect8 {
        static constexpr word width{8};
        static constexpr word index{0};
    };
}

#endif

