#ifndef LOADER_H
#define LOADER_H

#include "Hw/defs.h"
#include "Bootloader/command.h"

namespace App {
    struct SectionHeader {
        std::uint32_t base_address;
        std::uint32_t size;
    };

    enum setBlockErrorCodes: std::uint8_t {
        setBlockOverflow                = 0x01,
        setBlockMemoryLimitExceeded     = 0x02,
        setBlockStartPointerInvalid     = 0x03,
        setBlockDownloadSizeExceeded    = 0x04,
        setBlockBadChecksum             = 0x05,
        setBlockUnknownMemory           = 0x06,
    };

    enum continueBootErrorCodes: std::uint8_t {
        continueBootInvalidAddressError = 0x01
    };

    enum msgAckNack: std::uint8_t {
        msgAck = 0x5A,
        msgNack = 0xA5
    };

    class Loader
    {
    public:
        Loader(const std::vector<std::reference_wrapper<Hw::IMemory>> &memories, const Hw::IUart &uart);
        bool executeCommand();
        const Hw::IMemory& getTargetMemorySection() const;
        std::uintptr_t getBootAddress() const;
    private:
        const Hw::IUart &uart;
        const std::vector<std::reference_wrapper<Hw::IMemory>> &targetMemories;
        const std::array<Command, cmdNum> cmdTable;
        std::size_t selectedSection;
        std::uintptr_t bootAddress;

        bool setBlock();
        bool getInfo();
        bool continueBoot();
        bool initializeMemory();
        bool verifyBootAddress(std::uintptr_t bootAddress);

        constexpr static std::size_t MAX_DOWNLOAD_SIZE = 1024;
        std::array<std::uint8_t, MAX_DOWNLOAD_SIZE> dataBuffer;
    };
}

#endif // LOADER_H
