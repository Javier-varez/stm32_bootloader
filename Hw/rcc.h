#ifndef RCC_H
#define RCC_H

#include <cstdint>
#include "Hw/reg_access.h"

namespace Hw {
    template<std::uint32_t baseAddr>
    struct RccRegisterAddr {
        const static std::uint32_t CR =          baseAddr + 0x00;
        const static std::uint32_t PLLCFRG =     baseAddr + 0x04;
        const static std::uint32_t CFGR =        baseAddr + 0x08;
        const static std::uint32_t CIR =         baseAddr + 0x0C;
        const static std::uint32_t AHB1RSTR =    baseAddr + 0x10;
        const static std::uint32_t AHB2RSTR =    baseAddr + 0x14;
        const static std::uint32_t AHB3RSTR =    baseAddr + 0x18;
        const static std::uint32_t APB1RSTR =    baseAddr + 0x20;
        const static std::uint32_t APB2RSTR =    baseAddr + 0x24;
        const static std::uint32_t AHB1ENR =     baseAddr + 0x30;
        const static std::uint32_t AHB2ENR =     baseAddr + 0x34;
        const static std::uint32_t AHB3ENR =     baseAddr + 0x38;
        const static std::uint32_t APB1ENR =     baseAddr + 0x40;
        const static std::uint32_t APB2ENR =     baseAddr + 0x44;
        const static std::uint32_t AHB1LPENR =   baseAddr + 0x50;
        const static std::uint32_t AHB2LPENR =   baseAddr + 0x54;
        const static std::uint32_t AHB3LPENR =   baseAddr + 0x58;
        const static std::uint32_t APB1LPENR =   baseAddr + 0x60;
        const static std::uint32_t APB2LPENR =   baseAddr + 0x64;
        const static std::uint32_t BDCR =        baseAddr + 0x70;
        const static std::uint32_t CSR =         baseAddr + 0x74;
        const static std::uint32_t SSCGR =       baseAddr + 0x80;
        const static std::uint32_t PLLI2SCFGR =  baseAddr + 0x84;
        const static std::uint32_t PLLSAICFGR =  baseAddr + 0x88;
        const static std::uint32_t DCKCFGR1 =    baseAddr + 0x8C;
        const static std::uint32_t DCKCFGR2 =    baseAddr + 0x90;
    };

    template <typename regs>
    class Rcc {
    public:
        enum gpioBankIndex {
            A = 0,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K
        };
        static void enableGpioBank(gpioBankIndex bankId) {
            constexpr std::uint32_t addr = regs::AHB1ENR;
            reg_access<std::uint32_t, std::uint32_t, addr>::bit_set(bankId);
        }
        static void disableGpioBank(gpioBankIndex bankId) {
            constexpr std::uint32_t addr = regs::AHB1ENR;
            reg_access<std::uint32_t, std::uint32_t, addr>::bit_clr(bankId);
        }
        struct enableSet {
            std::uint32_t reg;
            std::uint32_t bit;
        };
        constexpr static enableSet usartEnableTable[] = {
            { regs::APB2ENR, 4 },
            { regs::APB1ENR, 17},
            { regs::APB1ENR, 18},
            { regs::APB1ENR, 19},
            { regs::APB1ENR, 20},
            { regs::APB2ENR, 5},
            { regs::APB1ENR, 30},
            { regs::APB1ENR, 31},
        };
        enum uartIndex {
            Uart1 = 0,
            Uart2,
            Uart3,
            Uart4,
            Uart5,
            Uart6,
            Uart7,
            Uart8
        };

        static void enableUart(uartIndex UartId) {
            const uint32_t reg = usartEnableTable[UartId].reg;
            const uint32_t val = usartEnableTable[UartId].bit;
            reg_access<std::uint32_t, std::uint32_t>::bit_set(reg, val);
        }
        static void disableUart(uartIndex UartId) {
            const uint32_t reg = usartEnableTable[UartId].reg;
            const uint32_t val = usartEnableTable[UartId].bit;
            reg_access<std::uint32_t, std::uint32_t>::bit_clr(reg, val);
        }

    };
}

#endif // RCC_H
