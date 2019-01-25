#ifndef UART_H
#define UART_H

#include "Hw/reg_access.h"
#include "Hw/gpio.h"
#include <string>
#include <algorithm>

namespace Hw {

    namespace UartConf {
        enum wordLength : std::uint32_t {
            b7 = 1 << 28,
            b8 = 0x00000000,
            b9 = 1 << 12,
            clear = b7 | b9
        };
        enum parityMode: std::uint32_t {
            none = 0x00000000,
            odd = 1 << 10 | 1 << 1,
            even = 1 << 10
        };
        enum baudRate: std::uint32_t {
            b9600 = 9600,
            b115200 = 115200
        };
        enum stopBits: std::uint32_t {
            b0_5 = 0b1 << 12,
            b1 = 0b0 << 12,
            b1_5 = 0b11 << 12,
            b2 = 0b10 << 12,
            stopBitsClr = b1_5
        };

        enum txEn: std::uint32_t {
            txEnable = 1 << 3,
            txDisable = 0 << 3
        };
        enum rxEn: std::uint32_t {
            rxEnable = 1 << 2,
            rxDisable = 0 << 2
        };
        enum En: std::uint32_t {
            Enable = 1 << 0,
            Disable = 0 << 0
        };
    }

    template<std::uint32_t base_addr, uCRcc::uartIndex uartId>
    struct UartRegisters {
        const static std::uint32_t CR1 = base_addr + 0x00;
        const static std::uint32_t CR2 = base_addr + 0x04;
        const static std::uint32_t CR3 = base_addr + 0x08;
        const static std::uint32_t BRR = base_addr + 0x0C;
        const static std::uint32_t GTPR = base_addr + 0x10;
        const static std::uint32_t RTOR = base_addr + 0x14;
        const static std::uint32_t RQR = base_addr + 0x18;
        const static std::uint32_t ISR = base_addr + 0x1C;
        const static std::uint32_t ICR = base_addr + 0x20;
        const static std::uint32_t RDR = base_addr + 0x24;
        const static std::uint32_t TDR = base_addr + 0x28;
        const static uCRcc::uartIndex id = uartId;
    };

    class IUart {
    public:
        std::uint8_t receive(std::uint32_t timeout_ms = 0) const {
            return receive_internal(timeout_ms);
        }
        template<typename T>
        T receive(std::uint32_t timeout_ms = 0) const {
            T retVal = 0;
            for (std::size_t i = 0; i < sizeof(T); i++) {
                retVal |= receive_internal(timeout_ms) << (i*8);
            }
            return retVal;
        }
        std::uint32_t receive(char* data, std::uint32_t length, std::uint32_t timeout_ms = 0) const {
            std::uint32_t i;
            for (i = 0; i < length; i++) {
                data[i] = receive_internal(timeout_ms);
                if ((data[i] == '\r') || (data[i] == '\n'))
                    break;
            }
            i++;
            return (i > length) ? length : i;
        }

        template<typename T>
        bool send(const T& data) const {
            for (std::size_t i = 0; i < sizeof(T); i++) {
                send_internal( ( data >> ( 8*i ) ) & 0xFF );
            }
            return true;
        }

        bool send(const char *data, const std::uint32_t length) const {
            for (unsigned int i = 0; i < length; i++) {
                send_internal(static_cast<std::uint8_t>(data[i]));
            }
            return true;
        }

        bool send(std::string str) const {
            std::for_each(str.begin(), str.end(), [this](auto c){
                send_internal(static_cast<std::uint8_t>(c));
            });
            return true;
        }
        virtual void deinitialize() = 0;
    private:
        virtual bool send_internal(const std::uint8_t data) const = 0;
        virtual std::uint8_t receive_internal(std::uint32_t timeout_ms) const = 0;
    protected:
        virtual ~IUart() {

        }
    };

    template <class regs,
              std::uint32_t clkRate,
              class rxGpio,
              class txGpio,
              GpioConfig::alternateModes rxGpioAFMode = GpioConfig::AF7,
              GpioConfig::alternateModes txGpioAFMode = GpioConfig::AF7,
              UartConf::wordLength defWordLength = UartConf::wordLength::b8,
              UartConf::parityMode defParityMode = UartConf::parityMode::none,
              UartConf::baudRate defBaudRate = UartConf::baudRate::b115200,
              UartConf::stopBits defStopBits = UartConf::stopBits::b1>
    class Uart: public IUart {
    private:
        // Delete copy constructor
        Uart(const Uart&) = delete;

        Uart() {
            rxGpio::enableBank();
            txGpio::enableBank();
            rxGpio::setAlternate(rxGpioAFMode);
            txGpio::setAlternate(txGpioAFMode);

            uCRcc::enableUart(regs::id);

            // set baud rate
            constexpr std::uint32_t BRR_val =
                    clkRate / defBaudRate;
            reg_access<std::uint32_t, std::uint32_t, regs::BRR, BRR_val>::reg_set();

            // Set stop bits
            std::uint32_t CR2_val = reg_access<std::uint32_t, std::uint32_t, regs::CR2>::reg_get();
            CR2_val &= ~(UartConf::stopBitsClr);
            CR2_val |= defStopBits;
            reg_access<std::uint32_t, std::uint32_t, regs::CR2>::reg_set(CR2_val);

            // set word length and parity (CR1) and enable uart, tx and rx
            constexpr std::uint32_t CR1_val =
                    UartConf::txEn::txEnable |
                    UartConf::rxEn::rxEnable |
                    UartConf::En::Enable |
                    defWordLength |
                    defParityMode;
            reg_access<std::uint32_t, std::uint32_t, regs::CR1, CR1_val>::reg_set();
        }

        ~Uart() {

        }

        bool send_internal(const std::uint8_t data) const override {
            while (!isTxEmpty());
            reg_access<std::uint32_t, std::uint8_t, regs::TDR>::reg_set(data);
            return true;
        }

        std::uint8_t receive_internal(std::uint32_t timeout_ms) const override {
            while (!isRxReady());
            std::uint32_t rdr = reg_access<std::uint32_t, std::uint32_t, regs::RDR>::reg_get();
            return static_cast<std::uint8_t>(rdr);
        }

        bool isRxReady() const {
            constexpr std::uint32_t RXNE_bit = 5;
            std::uint32_t rxne_val = reg_access<std::uint32_t, std::uint32_t, regs::ISR, RXNE_bit>::bit_get();
            return rxne_val != 0;
        }

        bool isTxEmpty() const {
            constexpr std::uint32_t TXE_bit = 7;
            std::uint32_t txe_val = reg_access<std::uint32_t, std::uint32_t, regs::ISR, TXE_bit>::bit_get();
            return txe_val != 0;
        }

        friend class factory;
    public:
        virtual void deinitialize() override {
            rxGpio::setMode(GpioConfig::inputMode);
            txGpio::setMode(GpioConfig::inputMode);
            rxGpio::disableBank();
            txGpio::disableBank();

            // set word length and parity (CR1) and enable uart, tx and rx
            constexpr std::uint32_t CR1_val =
                    UartConf::txEn::txDisable |
                    UartConf::rxEn::rxDisable |
                    UartConf::En::Disable |
                    defWordLength |
                    defParityMode;
            reg_access<std::uint32_t, std::uint32_t, regs::CR1, CR1_val>::reg_set();

            uCRcc::disableUart(regs::id);
        }
    };
}

#endif // UART_H
