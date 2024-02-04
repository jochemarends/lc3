/**
 * @file encoding.h
 * @author Jochem Arends
 */

#ifndef ENCODING_H
#define ENCODING_H

#include <lc3/types.h>

namespace lc3 {
    template<std::size_t N>
    word bit_at(std::integral auto bin) {
        return (bin >> N) & 1;
    }

    template<type T>
    word extract(word bin) {
        auto a = static_cast<word>(bin >> T::index);
        return zero_extend<T>(a);
    }

    template<type... Ts>
    auto decode(word bin) {
        return std::array<word, sizeof... (Ts)>{(extract<Ts>(bin))...};
    }
}

#endif

