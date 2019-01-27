#ifndef GPIO_H
#define GPIO_H

#include <cstdint>
#include "Hw/addr.h"
#include "Hw/reg_access.h"
#include "Hw/rcc.h"

namespace Hw {
    using uCRcc = Rcc<RccRegisterAddr<RCC_BASE>>;
    namespace GpioConfig {
        template <int base_address, uCRcc::gpioBankIndex id>
        struct Bank {
            const static std::uint32_t MODER =      base_address + 0;
            const static std::uint32_t OTYPER =     base_address + 4;
            const static std::uint32_t OSPEEDR =    base_address + 8;
            const static std::uint32_t PUPDR =      base_address + 12;
            const static std::uint32_t IDR =        base_address + 16;
            const static std::uint32_t ODR =        base_address + 20;
            const static std::uint32_t BSRR =       base_address + 24;
            const static std::uint32_t LCKR =       base_address + 28;
            const static std::uint32_t AFRL =       base_address + 32;
            const static std::uint32_t AFRH =       base_address + 36;
            const static uCRcc::gpioBankIndex bankId = id;
        };
        enum Bit : std::uint32_t {
            bit0 = 0,
            bit1,
            bit2,
            bit3,
            bit4,
            bit5,
            bit6,
            bit7,
            bit8,
            bit9,
            bit10,
            bit11,
            bit12,
            bit13,
            bit14,
            bit15
        };
        enum alternateModes: std::uint8_t {
            AF0 = 0,
            AF1,
            AF2,
            AF3,
            AF4,
            AF5,
            AF6,
            AF7,
            AF8,
            AF9,
            AF10,
            AF11,
            AF12,
            AF13,
            AF14,
            AF15
        };
        enum pinMode: std::uint32_t {
            inputMode = 0b00,
            outputMode = 0b01,
            alternateMode = 0b10,
            analogMode = 0b11,
            clrMode = analogMode
        };
    }


    template<typename port,
             GpioConfig::Bit bit>
    class Gpio {
    public:
        static void setMode(GpioConfig::pinMode mode) {
            std::uint32_t clearValue = ~(GpioConfig::clrMode << (2*bit));
            std::uint32_t modeValue = mode << (2*bit);
            constexpr std::uint32_t addr = port::MODER;

            reg_access<std::uint32_t, std::uint32_t, addr>::reg_and(clearValue);
            reg_access<std::uint32_t, std::uint32_t, addr>::reg_or(modeValue);
        }
        static void setAlternate(GpioConfig::alternateModes mode) {
            constexpr GpioConfig::Bit effectiveBit = static_cast<GpioConfig::Bit>((bit > 7) ? bit-8 : bit);

            constexpr std::uint32_t addr = (bit > 7) ? port::AFRH : port::AFRL;
            constexpr std::uint32_t clearValue = ~( GpioConfig::alternateModes::AF15 << (effectiveBit * 4) );
            std::uint32_t modeValue = mode << (effectiveBit * 4);

            reg_access<std::uint32_t, std::uint32_t, addr>::reg_and(clearValue);
            reg_access<std::uint32_t, std::uint32_t, addr>::reg_or(modeValue);

            setMode(GpioConfig::pinMode::alternateMode);
        }
        static void setOutput(bool on) {
            constexpr std::uint32_t onBit = 1 << bit;
            constexpr std::uint32_t offBit = 1 << (bit + 16);
            constexpr std::uint32_t addr = port::BSRR;
            if (on) {
                reg_access<std::uint32_t, std::uint32_t, addr, onBit>::reg_set();
            } else {
                reg_access<std::uint32_t, std::uint32_t, addr, offBit>::reg_set();
            }
        }
        static bool getValue() {
            constexpr std::uint32_t addr = port::IDR;
            return reg_access<std::uint32_t, std::uint32_t, addr, bit>::bit_get() != 0;
        }
        static void enableBank() {
            uCRcc::enableGpioBank(port::bankId);
        }
        static void disableBank() {
            uCRcc::disableGpioBank(port::bankId);
        }
    };
}

#endif // GPIO_H
