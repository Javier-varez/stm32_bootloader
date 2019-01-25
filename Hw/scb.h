#ifndef SCB_H
#define SCB_H

#include "Hw/gpio.h"

namespace Hw {
template<std::uint32_t base_addr>
struct SCBRegisters {
    const static std::uint32_t CPUID = base_addr + 0x000 ;
    const static std::uint32_t ICSR = base_addr + 0x004 ;
    const static std::uint32_t VTOR = base_addr + 0x008 ;
    const static std::uint32_t AIRCR = base_addr + 0x00C ;
    const static std::uint32_t SCR = base_addr + 0x010 ;
    const static std::uint32_t CCR = base_addr + 0x014 ;
    const static std::uint8_t  SHPR = base_addr + 0x018 ;
    const static std::uint32_t SHCSR = base_addr + 0x024 ;
    const static std::uint32_t CFSR = base_addr + 0x028 ;
    const static std::uint32_t HFSR = base_addr + 0x02C ;
    const static std::uint32_t DFSR = base_addr + 0x030 ;
    const static std::uint32_t MMFAR = base_addr + 0x034 ;
    const static std::uint32_t BFAR = base_addr + 0x038 ;
    const static std::uint32_t AFSR = base_addr + 0x03C ;
    const static std::uint32_t ID_PFR = base_addr + 0x040 ;
    const static std::uint32_t ID_DFR = base_addr + 0x048 ;
    const static std::uint32_t ID_AFR = base_addr + 0x04C ;
    const static std::uint32_t ID_MFR = base_addr + 0x050 ;
    const static std::uint32_t ID_ISAR = base_addr + 0x060 ;
    const static std::uint32_t CLIDR = base_addr + 0x078 ;
    const static std::uint32_t CTR = base_addr + 0x07C ;
    const static std::uint32_t CCSIDR = base_addr + 0x080 ;
    const static std::uint32_t CSSELR = base_addr + 0x084 ;
    const static std::uint32_t CPACR = base_addr + 0x088 ;
    const static std::uint32_t STIR = base_addr + 0x200 ;
    const static std::uint32_t MVFR0 = base_addr + 0x240 ;
    const static std::uint32_t MVFR1 = base_addr + 0x244 ;
    const static std::uint32_t MVFR2 = base_addr + 0x248 ;
    const static std::uint32_t ICIALLU = base_addr + 0x250 ;
    const static std::uint32_t ICIMVAU = base_addr + 0x258 ;
    const static std::uint32_t DCIMVAC = base_addr + 0x25C ;
    const static std::uint32_t DCISW = base_addr + 0x260 ;
    const static std::uint32_t DCCMVAU = base_addr + 0x264 ;
    const static std::uint32_t DCCMVAC = base_addr + 0x268 ;
    const static std::uint32_t DCCSW = base_addr + 0x26C ;
    const static std::uint32_t DCCIMVAC = base_addr + 0x270 ;
    const static std::uint32_t DCCISW = base_addr + 0x274 ;
    const static std::uint32_t ITCMCR = base_addr + 0x290 ;
    const static std::uint32_t DTCMCR = base_addr + 0x294 ;
    const static std::uint32_t AHBPCR = base_addr + 0x298 ;
    const static std::uint32_t CACR = base_addr + 0x29C ;
    const static std::uint32_t AHBSCR = base_addr + 0x2A0 ;
    const static std::uint32_t ABFSR = base_addr + 0x2A8 ;
};

template <class regs>
class SystemControlBlock {
public:
    static void enableVFUAccess() {
        constexpr std::uint32_t value = ((3UL << 10*2)|(3UL << 11*2));
        reg_access<std::uint32_t, std::uint32_t, regs::CPACR, value>::reg_or();
    }
    static void relocateVectorTable(std::uint32_t address) {
        __DMB();
        reg_access<std::uint32_t, std::uint32_t, regs::VTOR>::reg_set(address);
        __DSB();
    }
};
}

#endif // SCB_H
