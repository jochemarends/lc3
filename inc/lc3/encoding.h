#ifndef ENCODING_H
#define ENCODING_H

#include <lc3/types.h>

namespace lc3 {
    template<std::size_t N>
    std::uint16_t bit_at(std::integral auto bin) {
        return bin >> N & 1;
    }

    template<type T>
    std::uint16_t extract(std::uint16_t bin) {
        auto a = static_cast<std::uint16_t>(bin >> T::index);
        return zero_extend<T>(a);
    }

    template<type... Ts>
    auto decode(std::uint16_t bin) {
        using array_type = std::array<std::uint16_t, sizeof... (Ts)>;
        // return array_type{(static_cast<std::uint16_t>(bin >> Ts::index))...};
        return array_type{(extract<Ts>(bin))...};
    }
}

#endif

