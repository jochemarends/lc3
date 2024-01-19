#ifndef OPERANDS_H
#define OPERANDS_H

#include <cstdint>

namespace lc3 {
    template<typename T>
    concept operand = requires {
        {T::width} -> std::same_as<std::size_t>;
    };

    template<typename T>
    concept constant = operand<T> && requires {
        T::value;
    };

    template<operand... Operands> 
    struct encoding {
        
    };

    struct imm5 {
        static constexpr std::size_t width{5};
    };

    struct reg {
        static constexpr std::size_t width{3};
    };
}

#endif

