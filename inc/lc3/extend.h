#ifndef EXTEND_H
#define EXTEND_H

#include <lc3/types.h>
#include <cstdint>

namespace lc3 {
    template<type T>
    std::int16_t zero_extend(std::uint16_t value) {
        const auto msk = (1 << T::width) - 1;
        return msk & value;
    }

    template<type T>
    std::int16_t sign_extend(std::uint16_t value) {
        const auto msb = value >> (T::width - 1) & 1;
        const auto msk = ~(msb - 1) << T::width;
        return msk | zero_extend<T>(value);
    }
}

#endif
