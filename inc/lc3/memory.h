// tag::ignore[]

/**
 * @file memory.h
 * @author Jochem Arends
 */

// end::ignore[]

#ifndef MEMORY_H
#define MEMORY_H

#include <lc3/types.h>
#include <array>
#include <limits>

namespace lc3 {
    using memory = std::array<sword, 0x10000>;
}

#endif

