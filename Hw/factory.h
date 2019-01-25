#ifndef FACTORY_H
#define FACTORY_H

#include "Hw/defs.h"

namespace Hw {
    class factory
    {
    public:
        static Uart1& getUart1();
        static Uart6& getUart6();
        static uCSystemTimer& getSystemTimer();
    };
}

#endif // FACTORY_H
