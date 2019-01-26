#ifndef FACTORY_H
#define FACTORY_H

#include "Hw/defs.h"

namespace Hw {
    class factory
    {
    public:
        static Hw::IUart& getUart1();
        static Hw::IUart& getUart6();
        static Hw::uCSystemTimer& getSystemTimer();
        static Hw::IMemory& getFlashMemory();
        static Hw::IMemory& getRamMemory();
    };
}

#endif // FACTORY_H
