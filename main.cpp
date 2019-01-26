#include "Hw/factory.h"
#include "Bootloader/bootloader.h"
#include <vector>
#include <functional>

int main()
{
    std::vector<std::reference_wrapper<Hw::IMemory>> memories;
    memories.reserve(2);
    memories.emplace_back(Hw::factory::getRamMemory());
    memories.emplace_back(Hw::factory::getFlashMemory());

    App::Bootloader bootloader(memories);
    if (bootloader.load())
        bootloader.boot();

    while(1);
}
