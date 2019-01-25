#include "factory.h"

#include "Hw/defs.h"

Hw::Uart1& Hw::factory::getUart1() {
    static Hw::Uart1 uart1;
    return uart1;
}

Hw::Uart6& Hw::factory::getUart6() {
    static Hw::Uart6 uart6;
    return uart6;
}

Hw::uCSystemTimer& Hw::factory::getSystemTimer() {
    static Hw::uCSystemTimer systemTimer;
    return systemTimer;
}
