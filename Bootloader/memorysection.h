#ifndef MEMORYSECTION_H
#define MEMORYSECTION_H

#include <cstdint>

namespace App {
    struct memorySection {
        constexpr memorySection(std::uintptr_t addr, std::uint32_t length) :
            addr(addr),
            length(length)
        {

        }
        const std::uintptr_t addr;
        const std::uint32_t length;
    };
}

#endif // MEMORYSECTION_H
