#ifndef IMEMORY_H
#define IMEMORY_H

#include <cstdint>

namespace Hw {
class IMemory {
public:
    enum memoryType : std::uint32_t {
        RAM = 0,
        FLASH
    };

    IMemory() {

    }
    virtual ~IMemory() {

    }

    virtual std::uint32_t write(std::uintptr_t addr, std::uint8_t *data, std::size_t length) const = 0;
    virtual std::uint32_t read(std::uintptr_t addr, std::uint8_t *data, std::size_t length) const = 0;
    virtual bool initialize() const = 0;

    virtual bool validateAddressRange(std::uintptr_t addr, std::size_t length) const = 0;

    virtual memoryType getType() const = 0;
    virtual std::uintptr_t getBaseAddress() const = 0;
    virtual std::size_t getSize() const = 0;
};
}

#endif // IMEMORY_H
