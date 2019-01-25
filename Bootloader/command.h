#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <functional>
#include "Hw/uart.h"

namespace App {
    enum Command_t : std::uint8_t{
        cmdSetBlock,
        cmdGetInfo,
        cmdContinueBoot,
        cmdNum
    };

    struct Command {
        Command_t cmd;
        std::function<bool()> func;
    };
}

#endif // COMMAND_H
