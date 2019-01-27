#ifndef FLASH_H
#define FLASH_H

#include "Hw/imemory.h"
#include "Hw/reg_access.h"

namespace Hw {
    namespace Device {

        enum FlashSector: std::uint32_t {
            FlashSector0 = ( 1 << 0 ),
            FlashSector1 = ( 1 << 1 ),
            FlashSector2 = ( 1 << 2 ),
            FlashSector3 = ( 1 << 3 ),
            FlashSector4 = ( 1 << 4 ),
            FlashSector5 = ( 1 << 5 ),
            FlashSector6 = ( 1 << 6 ),
            FlashSector7 = ( 1 << 7 ),
            FlashSectorInvalid = ( 1 << 8 ),
        };
        constexpr std::uint32_t FlashSectorMax = 8;

        template <std::uint32_t regs_addr,
                  std::uint32_t mappedAddr,
                  FlashSector freeSectors>
        class Flash: public IMemory {
            template<std::uint32_t addr>
            struct FlashRegs {
                const static std::uintptr_t FLASH_ACR          = addr + 0x00;
                const static std::uintptr_t FLASH_KEYR         = addr + 0x04;
                const static std::uintptr_t FLASH_OPTKEYR      = addr + 0x08;
                const static std::uintptr_t FLASH_SR           = addr + 0x0C;
                const static std::uintptr_t FLASH_CR           = addr + 0x10;
                const static std::uintptr_t FLASH_OPTCR        = addr + 0x14;
                const static std::uintptr_t FLASH_OPTCR1       = addr + 0x18;
            };
            using regs = FlashRegs<regs_addr>;

            template<std::uint32_t base_address,
                     FlashSector sectors>
            struct FlashSectorManager {
                static bool isSectorActive(FlashSector sector)
                {
                    return sector & sectors;
                }
                static std::uint32_t getSectorSize(FlashSector sector) {
                    constexpr std::uint32_t smallSectorSize = 32 * 1024;
                    constexpr std::uint32_t mediumSectorSize = 128 * 1024;
                    constexpr std::uint32_t largeSectorSize = 256 * 1024;
                    switch (sector) {
                    case FlashSector0:
                    case FlashSector1:
                    case FlashSector2:
                    case FlashSector3:
                        return smallSectorSize;
                    case FlashSector4:
                        return mediumSectorSize;
                    case FlashSector5:
                    case FlashSector6:
                    case FlashSector7:
                        return largeSectorSize;
                    case FlashSectorInvalid:
                        return 0;
                    }
                    return 0;
                }
                static FlashSector getSectorForAddress(std::uintptr_t address) {
                    FlashSector sector = FlashSector0;
                    if (address < base_address)
                    {
                        return FlashSectorInvalid;
                    }

                    std::uintptr_t sectorEndPtr = base_address;
                    for (sector = FlashSector0; sector < FlashSectorInvalid;
                         sector = static_cast<FlashSector>(sector << 1)) {
                        sectorEndPtr += getSectorSize(sector);
                        if(address < sectorEndPtr) {
                            return sector;
                        }
                    }

                    return FlashSectorInvalid;
                }
                static bool checkAddressRange(std::uintptr_t start, std::size_t length) {
                    std::uintptr_t startAddress = start;
                    std::uintptr_t endAddress = start + length - 1;

                    FlashSector startSector = getSectorForAddress(startAddress);
                    FlashSector endSector = getSectorForAddress(endAddress);


                    for (FlashSector sector = startSector; sector <= endSector;
                         sector= static_cast<FlashSector>(sector << 1))
                    {
                        if (!isSectorActive(sector)) {
                            return false;
                        }
                    }
                    return true;
                }
                static std::uint32_t getAllSectorsSize() {
                    std::uint32_t totalSize = 0;
                    for (FlashSector sector = FlashSector0; sector < FlashSectorInvalid;
                         sector = static_cast<FlashSector>(sector << 1)) {
                        if (isSectorActive(sector)) {
                            totalSize += getSectorSize(sector);
                        }
                    }
                    return totalSize;
                }
            };
            using SectorManager = FlashSectorManager<mappedAddr, freeSectors>;

            enum FlashUnlockValues: std::uint32_t {
                KEY0 = 0x45670123,
                KEY1 = 0xCDEF89AB,
            };

            enum FlashSRBits: std::uint32_t {
                BSY      = UINT32_C(1) << 16,
                ERSERR   = UINT32_C(1) << 7,
                PGPERR   = UINT32_C(1) << 6,
                PGAERR   = UINT32_C(1) << 5,
                WRPERR   = UINT32_C(1) << 4,
                OPERR    = UINT32_C(1) << 1,
                EOP      = UINT32_C(1) << 0,
            };
            enum FlashCRPos: std::uint32_t {
                LOCK_POS     = 31,
                ERRIE_POS    = 25,
                EOPIE_POS    = 24,
                STRT_POS     = 16,
                PSIZE1_POS   = 9,
                PSIZE0_POS   = 8,
                SNB3_POS     = 6,
                SNB2_POS     = 5,
                SNB1_POS     = 4,
                SNB0_POS     = 3,
                MER_POS      = 2,
                SER_POS      = 1,
                PG_POS       = 0
            };
            enum FlashCRBits: std::uint32_t {
                LOCK     = UINT32_C(1) << LOCK_POS,
                ERRIE    = UINT32_C(1) << ERRIE_POS,
                EOPIE    = UINT32_C(1) << EOPIE_POS,
                STRT     = UINT32_C(1) << STRT_POS,
                PSIZE1   = UINT32_C(1) << PSIZE1_POS,
                PSIZE0   = UINT32_C(1) << PSIZE0_POS,
                SNB3     = UINT32_C(1) << SNB3_POS,
                SNB2     = UINT32_C(1) << SNB2_POS,
                SNB1     = UINT32_C(1) << SNB1_POS,
                SNB0     = UINT32_C(1) << SNB0_POS,
                MER      = UINT32_C(1) << MER_POS,
                SER      = UINT32_C(1) << SER_POS,
                PG       = UINT32_C(1) << PG_POS,
                SNBALL   = SNB0 | SNB1 | SNB2 | SNB3,
                PSIZEALL = PSIZE0 | PSIZE1
            };

            void waitUntilReady( void ) const
            {
                bool Busy = false;
                do {
                    std::uint32_t SR = Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_SR>::reg_get();
                    Busy = SR & BSY;
                } while(Busy);
            }

            void unlockFlash( void ) const
            {
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_KEYR, KEY0>::reg_set();
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_KEYR, KEY1>::reg_set();
            }

            void lockFlash( void ) const
            {
                clearCtrlRegister();
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_CR>::reg_or(LOCK);
            }

            void clearCtrlRegister( void ) const
            {
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_CR>::reg_and(~(SNBALL | PSIZEALL | MER | SER | PG));
            }

            void eraseSector( std::uint32_t sector ) const
            {
                waitUntilReady();
                unlockFlash();
                clearCtrlRegister();

                std::uint32_t CR = (sector << SNB0_POS) | SER;
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_CR>::reg_or(CR);
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_CR>::reg_or(STRT);

                waitUntilReady();
                lockFlash();
            }

            Flash()
            {

            }

            friend class Hw::factory;

        public:

            virtual bool validateAddressRange(std::uintptr_t addr, std::size_t length) const override {
                return SectorManager::checkAddressRange(addr, length);
            }

            virtual bool initialize( void ) const override {
                static bool alreadyInitialized = false;
                if (alreadyInitialized)
                    return true;

                for (std::uint32_t i = 0; i < FlashSectorMax; i++) {
                    if (freeSectors & (1 << i)) {
                        eraseSector(i);
                    }
                }
                alreadyInitialized = true;
                return true;
            }

            virtual std::uint32_t read( std::uintptr_t addr, std::uint8_t *data, std::size_t length ) const override {
                if (!validateAddressRange(addr, length)) {
                    return 0;
                }

                for (std::uint32_t i = 0; i < length; i++) {
                    data[i] = Hw::reg_access<std::uintptr_t, uint8_t>::reg_get(addr+i);
                }
                return length;
            }

            virtual std::uint32_t write( std::uintptr_t addr, std::uint8_t *data, std::size_t length ) const override {
                if (!validateAddressRange(addr, length)) {
                    return 0;
                }

                waitUntilReady();
                unlockFlash();
                clearCtrlRegister();
                Hw::reg_access<std::uintptr_t, std::uint32_t, regs::FLASH_CR>::reg_or(PG);

                for (std::size_t i = 0; i < length; i++) {
                    Hw::reg_access<std::uintptr_t, std::uint8_t>::reg_set(addr + i, data[i]);
                    /* We can use a Data Syncrhonization Barrier to wait for flash to be available again */
                    __DSB();
                }

                waitUntilReady();
                lockFlash();
                return length;
            }

            virtual memoryType getType() const override {
                return Hw::IMemory::memoryType::FLASH;
            }

            virtual std::uintptr_t getBaseAddress() const override {
                /* Returns first available sector address */
                std::uint32_t addedSize = 0;
                for (FlashSector sector = FlashSector0; sector != FlashSectorInvalid;
                     sector = static_cast<FlashSector>(sector << 1)) {
                    if (!SectorManager::isSectorActive(sector)) {
                        addedSize += SectorManager::getSectorSize(sector);
                    }
                    else {
                        break;
                    }
                }
                return mappedAddr + addedSize;
            }
            virtual std::size_t getSize() const override {
                return SectorManager::getAllSectorsSize();
            }
        };
    }
}



#endif // FLASH_H
