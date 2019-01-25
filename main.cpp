#include <cstdint>
#include "Hw/factory.h"

#include "Bootloader/bootloader.h"

constexpr std::uintptr_t RAM_LOAD_BASE = UINT32_C(0x20000000);
constexpr std::uint32_t RAM_LENGTH = UINT32_C(1024 * 200);
constexpr App::memorySection RAM(RAM_LOAD_BASE, RAM_LENGTH);

constexpr std::uintptr_t FLASH_LOAD_BASE = UINT32_C(0x08008000);
constexpr std::uint32_t FLASH_LENGTH = UINT32_C(1024 * 992);
constexpr App::memorySection FLASH(FLASH_LOAD_BASE, FLASH_LENGTH);

int main()
{
    App::Bootloader bootloader(RAM);
    if (bootloader.load())
        bootloader.boot();

    while(1);
}
