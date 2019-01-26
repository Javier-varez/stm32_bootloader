#include "factory.h"

#include "Hw/defs.h"

Hw::IUart& Hw::factory::getUart1() {
    static Hw::Uart1 uart1;
    return uart1;
}

Hw::IUart& Hw::factory::getUart6() {
    static Hw::Uart6 uart6;
    return uart6;
}

Hw::uCSystemTimer& Hw::factory::getSystemTimer() {
    static Hw::uCSystemTimer systemTimer;
    return systemTimer;
}

Hw::IMemory & Hw::factory::getFlashMemory() {
    static Hw::uCFlash flash;
    return flash;
}

Hw::IMemory & Hw::factory::getRamMemory() {
    static Hw::uCRam ram;
    return ram;
}
