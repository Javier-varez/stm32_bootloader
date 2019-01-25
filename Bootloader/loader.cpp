#include "Bootloader/loader.h"
#include "Hw/factory.h"

App::Loader::Loader(const memorySection& mem, const Hw::IUart &uart) :
    uart(uart),
    targetMemory(mem),
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
            }
        }
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

template <class T>
static std::uint16_t fletcherCRC(const T& data, std::size_t size) {
    std::uint16_t fletcher = 0x00;
    std::uint8_t C0 = 0;
    std::uint8_t C1 = 0;

    for (std::size_t i = 0; i < size; i++)
    {
        C0 = ( C0 + data[i] ) % 255;
        C1 = ( C1 + C0 ) % 255;
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
    } else if (start_ptr < targetMemory.addr) {
        uart.send(msgNack);
        uart.send(setBlockStartPointerInvalid);
        return done;
    } else if (end_ptr > (targetMemory.addr + targetMemory.length)) {
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
    std::copy_n(dataBuffer.begin(), header.size,
                reinterpret_cast<std::uint8_t*>(start_ptr));

    uart.send(msgAck);
    return done;
}

bool App::Loader::getInfo() {
    uart.send(msgAck);
    uart.send(targetMemory.addr);
    uart.send(targetMemory.length);
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
