#ifndef RAM_H
#define RAM_H

#include "Hw/imemory.h"
#include "Hw/reg_access.h"

namespace Hw {
namespace Device {

    template <std::uintptr_t base_addr,
              std::size_t memLength>
    class Ram: public IMemory {
    private:
        //Private constructor
        Ram() {

        }


    public:
        virtual bool validateAddressRange(std::uintptr_t addr, std::size_t length) const override {
            std::uintptr_t start = addr;
            std::uintptr_t end = addr + length - 1;

            std::uintptr_t endMemory = base_addr + memLength - 1;
            if ((start < base_addr) || (start > endMemory) || (end < base_addr) || (end > endMemory)) {
                return false;
            }
            return true;
        }

        virtual std::uint32_t write(std::uintptr_t addr, std::uint8_t *data, std::size_t length) const override {
            if (!validateAddressRange(addr, length)) {
                return 0;
            }
            for (std::uint32_t i = 0; i < length; i++) {
                Hw::reg_access<std::uintptr_t, std::uint8_t>::reg_set(addr + i, data[i]);
            }
            return length;
        }

        virtual std::uint32_t read(std::uintptr_t addr, std::uint8_t *data, std::size_t length) const override {
            if (!validateAddressRange(addr, length)) {
                return 0;
            }
            for (std::uint32_t i = 0; i < length; i++) {
                data[i] = Hw::reg_access<std::uintptr_t, std::uint8_t>::reg_get(addr + i);
            }
            return length;
        }

        virtual bool initialize() const override {
            return true;
        }

        virtual memoryType getType() const override {
            return Hw::IMemory::memoryType::RAM;
        }

        virtual std::uintptr_t getBaseAddress() const override {
            return base_addr;
        }

        virtual std::size_t getSize() const override {
            return memLength;
        }
        friend class Hw::factory;
    };

}
}

#endif // RAM_H
