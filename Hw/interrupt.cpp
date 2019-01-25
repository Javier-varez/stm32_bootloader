#include "Hw/factory.h"

extern "C" __attribute__((interrupt)) void SysTick_Handler() {
    Hw::uCSystemTimer &timer = Hw::factory::getSystemTimer();
    timer.increment();
}
