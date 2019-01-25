#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <cstdint>
#include "bootloader/memorysection.h"
#include "bootloader/loader.h"

namespace App {
class Bootloader
{
public:
    Bootloader(const memorySection &mem);
    bool load();
    void boot();
private:
    memorySection targetMemory;
    Hw::IUart &uart;
    Loader loader;
    void disablePeripherals();
};
}

#endif // BOOTLOADER_H
