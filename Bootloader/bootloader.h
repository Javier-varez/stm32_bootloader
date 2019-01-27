#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <cstdint>
#include "bootloader/loader.h"

namespace App {
class Bootloader
{
public:
    Bootloader(const std::vector<std::reference_wrapper<Hw::IMemory>>& memories);
    bool load();
    void boot();
    void boot(std::uintptr_t bootAddress);
private:
    const std::vector<std::reference_wrapper<Hw::IMemory>>& targetMemories;
    Hw::IUart &uart;
    Loader loader;
    void disablePeripherals();
};
}

#endif // BOOTLOADER_H
