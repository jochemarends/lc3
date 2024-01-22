#ifndef MEMORY_H
#define MEMORY_H

namespace lc3 {
    struct memory {
        using sword = std::int16_t;
        using word = std::uint16_t;

        sword& operator[](word idx) {
            return m_data[idx];

        }

        sword m_data[0xFFFF]{};

        sword* data() {
            return m_data;
        }
    };
}

#endif

