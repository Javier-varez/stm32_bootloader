#include "bootloader.h"
#include "Hw/factory.h"

App::Bootloader::Bootloader(const App::memorySection& mem) :
    targetMemory(mem),
    uart(Hw::factory::getUart1()),
    loader(targetMemory, uart)
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

void App::Bootloader::boot() {
    disablePeripherals();

    Hw::uCSystemControlBlock::relocateVectorTable(targetMemory.addr);

    // Set Stack pointer and jump to target
    asm("ldr sp, =_estack\r\n"
        "ldr pc, [%0, #4]\r\n" : : "r" (targetMemory.addr));
}
