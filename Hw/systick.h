#ifndef SYSTICK_H
#define SYSTICK_H

#include <cstdint>
#include "Hw/addr.h"
#include "Hw/reg_access.h"

namespace Hw {
template<std::uint32_t base_addr>
struct SysTickRegisters {
    const static std::uint32_t CTRL  = base_addr + 0x00;
    const static std::uint32_t LOAD  = base_addr + 0x04;
    const static std::uint32_t VAL   = base_addr + 0x08;
    const static std::uint32_t CALIB = base_addr + 0x0C;
};

namespace SysTickConfig {
    enum clkSrc {
        implementationDefined = 0 << 2,
        processorClock = 1 << 2
    };
    enum irqMode {
        disableIrq = 0 << 1,
        enableIrq = 1 << 1
    };
    enum enableMode {
        disableSysTick = 0 << 0,
        enableSysTick = 1 << 0
    };
}

template<class regs,
         std::uint32_t inputClk,
         std::uint32_t ticks>
class SysTickTimer {
public:
    static void initialize() {
        setFrequency();
        enable();
    }
    static void deinitialize() {
        disable();
    }
    constexpr static std::uint32_t getTicks() {
        return ticks;
    }
private:
    static void setFrequency() {
        static_assert (inputClk > ticks, "inputClk is not larger than ticks");
        constexpr std::uint32_t autoReloadValue = inputClk / ticks;
        constexpr std::uint32_t max24bitValue = (1 << 24) - 1;
        static_assert (autoReloadValue <= max24bitValue,
                       "autoReloadValue is larger than the max for 24 bits SysTick Timer");
        reg_access<std::uint32_t, std::uint32_t, regs::LOAD, autoReloadValue-1>::reg_set();
        reg_access<std::uint32_t, std::uint32_t, regs::VAL, 0>::reg_set();
    }
    static void enable() {
        constexpr std::uint32_t value = SysTickConfig::processorClock |
                SysTickConfig::enableIrq |
                SysTickConfig::enableSysTick;
        reg_access<std::uint32_t, std::uint32_t, regs::CTRL, value>::reg_set();

        //Set IRQ priority in the NVIC
        NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    }
    static void disable() {
        constexpr std::uint32_t value = SysTickConfig::processorClock |
                SysTickConfig::disableIrq |
                SysTickConfig::disableSysTick;
        reg_access<std::uint32_t, std::uint32_t, regs::CTRL, value>::reg_set();
    }
};

}

#endif // SYSTICK_H
