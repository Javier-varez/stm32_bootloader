#include "Bootloader/loader.h"
#include "Hw/factory.h"

App::Loader::Loader(const std::vector<std::reference_wrapper<Hw::IMemory>>& memories, const Hw::IUart &uart) :
    uart(uart),
    targetMemories(memories),
    cmdTable
        {
            {
                {
                    App::cmdSetBlock,
                    std::bind(&App::Loader::setBlock, this),
                },
                {
                    App::cmdGetInfo,
                    std::bind(&App::Loader::getInfo, this),
                },
                {
                    App::cmdContinueBoot,
                    std::bind(&App::Loader::ContinueBoot, this),
                },
                {
                    App::cmdGetSections,
                    std::bind(&App::Loader::getSections, this),
                },
                {
                    App::cmdSetTargetSection,
                    std::bind(&App::Loader::setTargetSection, this),
                },
            }
        },
    selectedSection(0)
{

}

bool App::Loader::executeCommand() {
    auto recvCmd = static_cast<Command_t>(uart.receive());
    bool done = false;

    std::for_each(cmdTable.begin(), cmdTable.end(), [recvCmd, &done](Command cmd){
        if (recvCmd == cmd.cmd)
            done = cmd.func();
    });
    return !done;
}

const Hw::IMemory& App::Loader::getTargetMemorySection() const {
    return targetMemories[selectedSection].get();
}


template <class T>
static std::uint16_t fletcherCRC(const T& data, std::size_t size) {
    std::uint16_t fletcher = 0x00;
    std::uint8_t C0 = 0;
    std::uint8_t C1 = 0;

    for (std::size_t i = 0; i < size; i++)
    {
        C0 = ( C0 + data[i] ) & 0xFF;
        C1 = ( C1 + C0 ) & 0xFF;
    }

    fletcher = static_cast<std::uint16_t>((C1 << 8) + (C0 << 0));
    return fletcher;
}

bool App::Loader::setBlock() {
    bool done = false;
    SectionHeader header;

    header.base_address = uart.receive<std::uint32_t>();
    header.size = uart.receive<std::uint32_t>();

    // Make sure we don't overflow the available memory regions
    std::uint32_t start_ptr = header.base_address;
    std::uint32_t end_ptr = header.base_address + header.size;
    if (header.size > MAX_DOWNLOAD_SIZE) {
        uart.send(msgNack);
        uart.send(setBlockDownloadSizeExceeded);
        return done;
    } else if (end_ptr < start_ptr) {
        uart.send(msgNack);
        uart.send(setBlockOverflow);
        return done;
    } else if (start_ptr < getTargetMemorySection().getBaseAddress()) {
        uart.send(msgNack);
        uart.send(setBlockStartPointerInvalid);
        return done;
    } else if (end_ptr > (getTargetMemorySection().getBaseAddress() + getTargetMemorySection().getSize())) {
        uart.send(msgNack);
        uart.send(setBlockMemoryLimitExceeded);
        return done;
    }

    // Start data reception
    uart.send(msgAck);
    for (std::size_t i = 0; i < header.size; i++) {
        dataBuffer[i] = uart.receive();
    }

    // Test Checksum
    std::uint16_t checksum = uart.receive<std::uint16_t>();
    if ( checksum != fletcherCRC(dataBuffer, header.size)) {
        uart.send(msgNack);
        uart.send(setBlockBadChecksum);
        return done;
    }

    // Perform copy into memory
    getTargetMemorySection().write(start_ptr, dataBuffer.begin(), header.size);

    uart.send(msgAck);
    return done;
}

bool App::Loader::getInfo() {
    uart.send(msgAck);
    uart.send(getTargetMemorySection().getBaseAddress());
    uart.send(getTargetMemorySection().getSize());
    uart.send(MAX_DOWNLOAD_SIZE);
    return false;
}

bool App::Loader::ContinueBoot() {
    uart.send(msgAck);
    // We need to wait for a bit or we will deinit the uart before the Ack is sent.
    Hw::uCSystemTimer &timer = Hw::factory::getSystemTimer();
    timer.delay(5);
    return true;
}

bool App::Loader::getSections() {
    uart.send(msgAck);
    uart.send<uint32_t>(targetMemories.size());

    for (std::size_t i = 0; i < targetMemories.size(); i++) {
        uart.send(targetMemories[i].get().getBaseAddress());
        uart.send(targetMemories[i].get().getSize());
        uart.send(targetMemories[i].get().getType());
    }
    return false;
}

bool App::Loader::setTargetSection() {
    std::uint8_t targetSection = uart.receive<std::uint8_t>();
    if (targetSection >= targetMemories.size()) {
        uart.send(msgNack);
    } else {
        uart.send(msgAck);
        selectedSection = targetSection;
    }
    return false;
}
