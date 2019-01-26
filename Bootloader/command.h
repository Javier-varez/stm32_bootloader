#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <functional>

namespace App {
    enum Command_t : std::uint8_t{
        cmdSetBlock,
        cmdGetInfo,
        cmdContinueBoot,
        cmdGetSections,
        cmdSetTargetSection,
        cmdNum
    };

    struct Command {
        Command_t cmd;
        std::function<bool()> func;
    };
}

#endif // COMMAND_H
