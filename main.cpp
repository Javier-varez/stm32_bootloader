#include "Hw/factory.h"
#include "Bootloader/bootloader.h"
#include <vector>
#include <functional>

using userButton = Hw::gpioI11;
using led = Hw::gpioI1;

int main()
{
    std::vector<std::reference_wrapper<Hw::IMemory>> memories({Hw::factory::getRamMemory(), Hw::factory::getFlashMemory()});
    App::Bootloader bootloader(memories);

    userButton::enableBank();
    userButton::setMode(Hw::GpioConfig::pinMode::inputMode);
    led::enableBank();
    led::setMode(Hw::GpioConfig::pinMode::outputMode);
    led::setOutput(false);

    if (userButton::getValue()) {
        led::setOutput(true);
        bootloader.load();
    }
    bootloader.boot();

    while(1);
}
