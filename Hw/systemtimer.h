#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

#include "Hw/systick.h"
#include "Hw/interrupt.h"

namespace Hw {
class factory;

template <class systick>
class SystemTimer {
public:
    void delay(std::uint32_t ms) const
    {
        constexpr std::uint32_t ticksPerSecond = systick::getTicks();
        std::uint32_t tickStart = ticks;
        std::uint32_t delayTick = (ticksPerSecond * static_cast<std::uint64_t>(ms)) / 1000;

        while ((ticks - tickStart) < delayTick);
    }

    std::uint32_t getCount() const
    {
        return ticks;
    }

    std::uint32_t getTimeMs() const {
        constexpr std::uint32_t ticksPerSecond = systick::getTicks();
        return (static_cast<std::uint64_t>(ticks) * 1000) / ticksPerSecond;
    }
    void deinitialize() {
        systick::deinitialize();
    }
private:
    volatile uint32_t ticks;

    SystemTimer() :
        ticks(0)
    {
        systick::initialize();
    }
    SystemTimer(const SystemTimer&) = delete;

    ~SystemTimer()
    {
        deinitialize();
    }

    void increment()
    {
        ticks++;
    }

    friend class Hw::factory;
    friend void ::SysTick_Handler();
};

}

#endif // SYSTEMTIMER_H
