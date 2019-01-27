#include "bootloader.h"
#include "Hw/factory.h"

App::Bootloader::Bootloader(const std::vector<std::reference_wrapper<Hw::IMemory>>& memories) :
    targetMemories(memories),
    uart(Hw::factory::getUart1()),
    loader(targetMemories, uart)
{

}

bool App::Bootloader::load() {
    while (loader.executeCommand());
    return true;
}

void App::Bootloader::disablePeripherals() {
    Hw::uCSystemTimer &systemTimer = Hw::factory::getSystemTimer();
    systemTimer.deinitialize();
    uart.deinitialize();
}

void App::Bootloader::boot(std::uintptr_t addr) {
    disablePeripherals();

    Hw::uCSystemControlBlock::relocateVectorTable(addr);

    // Set Stack pointer and jump to target
    asm("ldr sp, =_estack\r\n"
        "ldr pc, [%0, #4]\r\n" : : "r" (addr));
}

void App::Bootloader::boot() {
    std::uintptr_t bootAddress = loader.getBootAddress();
    boot(bootAddress);
}
