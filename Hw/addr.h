#ifndef ADDR_H
#define ADDR_H

#include <cstdint>

typedef enum
{
/******  Cortex-M7 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M7 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M7 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M7 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M7 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M7 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M7 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M7 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
  DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
  DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
  DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
  DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
  DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
  DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
  TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
  TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
  TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
  TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
  FMC_IRQn                    = 48,     /*!< FMC global Interrupt                                              */
  SDMMC1_IRQn                 = 49,     /*!< SDMMC1 global Interrupt                                           */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
  DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
  DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
  DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
  DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
  ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
  ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
  CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
  CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
  CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
  CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
  DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
  DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
  USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
  OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
  OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
  OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
  DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
  RNG_IRQn                    = 80,     /*!< RNG global interrupt                                              */
  FPU_IRQn                    = 81,     /*!< FPU global interrupt                                              */
  UART7_IRQn                  = 82,     /*!< UART7 global interrupt                                            */
  UART8_IRQn                  = 83,     /*!< UART8 global interrupt                                            */
  SPI4_IRQn                   = 84,     /*!< SPI4 global Interrupt                                             */
  SPI5_IRQn                   = 85,     /*!< SPI5 global Interrupt                                             */
  SPI6_IRQn                   = 86,     /*!< SPI6 global Interrupt                                             */
  SAI1_IRQn                   = 87,     /*!< SAI1 global Interrupt                                             */
  LTDC_IRQn                   = 88,     /*!< LTDC global Interrupt                                             */
  LTDC_ER_IRQn                = 89,     /*!< LTDC Error global Interrupt                                       */
  DMA2D_IRQn                  = 90,     /*!< DMA2D global Interrupt                                            */
  SAI2_IRQn                   = 91,     /*!< SAI2 global Interrupt                                             */
  QUADSPI_IRQn                = 92,     /*!< Quad SPI global interrupt                                         */
  LPTIM1_IRQn                 = 93,     /*!< LP TIM1 interrupt                                                 */
  CEC_IRQn                    = 94,     /*!< HDMI-CEC global Interrupt                                         */
  I2C4_EV_IRQn                = 95,     /*!< I2C4 Event Interrupt                                              */
  I2C4_ER_IRQn                = 96,     /*!< I2C4 Error Interrupt                                              */
  SPDIF_RX_IRQn               = 97      /*!< SPDIF-RX global Interrupt                                         */
} IRQn_Type;

#define __CM7_REV                 0x0001  /*!< Cortex-M7 revision r0p1                       */
#define __MPU_PRESENT             1       /*!< CM7 provides an MPU                           */
#define __NVIC_PRIO_BITS          4       /*!< CM7 uses 4 Bits for the Priority Levels       */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1       /*!< FPU present                                   */
#define __ICACHE_PRESENT          1       /*!< CM7 instruction cache present                 */
#define __DCACHE_PRESENT          1       /*!< CM7 data cache present                        */
#include "core_cm7.h"

constexpr std::uint32_t RAMITCM_BASE           = 0x00000000; /*!< Base address of :16KB RAM reserved for CPU execution/instruction accessible over ITCM   */
constexpr std::uint32_t FLASHITCM_BASE         = 0x00200000; /*!< Base address of :(up to 1 MB) embedded FLASH memory  accessible over ITCM               */
constexpr std::uint32_t FLASHAXI_BASE          = 0x08000000; /*!< Base address of : (up to 1 MB) embedded FLASH memory accessible over AXI                */
constexpr std::uint32_t RAMDTCM_BASE           = 0x20000000; /*!< Base address of : 64KB system data RAM accessible over DTCM                             */
constexpr std::uint32_t PERIPH_BASE            = 0x40000000; /*!< Base address of : AHB/ABP Peripherals                                                   */
constexpr std::uint32_t BKPSRAM_BASE           = 0x40024000; /*!< Base address of : Backup SRAM(4 KB)                                                     */
constexpr std::uint32_t QSPI_BASE              = 0x90000000; /*!< Base address of : QSPI memories  accessible over AXI                                    */
constexpr std::uint32_t FMC_R_BASE             = 0xA0000000; /*!< Base address of : FMC Control registers                                                 */
constexpr std::uint32_t QSPI_R_BASE            = 0xA0001000; /*!< Base address of : QSPI Control  registers                                               */
constexpr std::uint32_t SRAM1_BASE             = 0x20010000; /*!< Base address of : 240KB RAM1 accessible over AXI/AHB                                    */
constexpr std::uint32_t SRAM2_BASE             = 0x2004C000; /*!< Base address of : 16KB RAM2 accessible over AXI/AHB                                     */
constexpr std::uint32_t FLASH_END              = 0x080FFFFF; /*!< FLASH end address */

/* Legacy define */
constexpr std::uint32_t FLASH_BASE    = FLASHAXI_BASE;

/*!< Peripheral memory map */
constexpr std::uint32_t APB1PERIPH_BASE       = PERIPH_BASE;
constexpr std::uint32_t APB2PERIPH_BASE       = PERIPH_BASE + 0x00010000;
constexpr std::uint32_t AHB1PERIPH_BASE       = PERIPH_BASE + 0x00020000;
constexpr std::uint32_t AHB2PERIPH_BASE       = PERIPH_BASE + 0x10000000;

/*!< APB1 peripherals */
constexpr std::uint32_t TIM2_BASE             = APB1PERIPH_BASE + 0x0000;
constexpr std::uint32_t TIM3_BASE             = APB1PERIPH_BASE + 0x0400;
constexpr std::uint32_t TIM4_BASE             = APB1PERIPH_BASE + 0x0800;
constexpr std::uint32_t TIM5_BASE             = APB1PERIPH_BASE + 0x0C00;
constexpr std::uint32_t TIM6_BASE             = APB1PERIPH_BASE + 0x1000;
constexpr std::uint32_t TIM7_BASE             = APB1PERIPH_BASE + 0x1400;
constexpr std::uint32_t TIM12_BASE            = APB1PERIPH_BASE + 0x1800;
constexpr std::uint32_t TIM13_BASE            = APB1PERIPH_BASE + 0x1C00;
constexpr std::uint32_t TIM14_BASE            = APB1PERIPH_BASE + 0x2000;
constexpr std::uint32_t LPTIM1_BASE           = APB1PERIPH_BASE + 0x2400;
constexpr std::uint32_t RTC_BASE              = APB1PERIPH_BASE + 0x2800;
constexpr std::uint32_t WWDG_BASE             = APB1PERIPH_BASE + 0x2C00;
constexpr std::uint32_t IWDG_BASE             = APB1PERIPH_BASE + 0x3000;
constexpr std::uint32_t SPI2_BASE             = APB1PERIPH_BASE + 0x3800;
constexpr std::uint32_t SPI3_BASE             = APB1PERIPH_BASE + 0x3C00;
constexpr std::uint32_t SPDIFRX_BASE          = APB1PERIPH_BASE + 0x4000;
constexpr std::uint32_t USART2_BASE           = APB1PERIPH_BASE + 0x4400;
constexpr std::uint32_t USART3_BASE           = APB1PERIPH_BASE + 0x4800;
constexpr std::uint32_t UART4_BASE            = APB1PERIPH_BASE + 0x4C00;
constexpr std::uint32_t UART5_BASE            = APB1PERIPH_BASE + 0x5000;
constexpr std::uint32_t I2C1_BASE             = APB1PERIPH_BASE + 0x5400;
constexpr std::uint32_t I2C2_BASE             = APB1PERIPH_BASE + 0x5800;
constexpr std::uint32_t I2C3_BASE             = APB1PERIPH_BASE + 0x5C00;
constexpr std::uint32_t I2C4_BASE             = APB1PERIPH_BASE + 0x6000;
constexpr std::uint32_t CAN1_BASE             = APB1PERIPH_BASE + 0x6400;
constexpr std::uint32_t CAN2_BASE             = APB1PERIPH_BASE + 0x6800;
constexpr std::uint32_t CEC_BASE              = APB1PERIPH_BASE + 0x6C00;
constexpr std::uint32_t PWR_BASE              = APB1PERIPH_BASE + 0x7000;
constexpr std::uint32_t DAC_BASE              = APB1PERIPH_BASE + 0x7400;
constexpr std::uint32_t UART7_BASE            = APB1PERIPH_BASE + 0x7800;
constexpr std::uint32_t UART8_BASE            = APB1PERIPH_BASE + 0x7C00;

/*!< APB2 peripherals */
constexpr std::uint32_t TIM1_BASE             = APB2PERIPH_BASE + 0x0000;
constexpr std::uint32_t TIM8_BASE             = APB2PERIPH_BASE + 0x0400;
constexpr std::uint32_t USART1_BASE           = APB2PERIPH_BASE + 0x1000;
constexpr std::uint32_t USART6_BASE           = APB2PERIPH_BASE + 0x1400;
constexpr std::uint32_t ADC1_BASE             = APB2PERIPH_BASE + 0x2000;
constexpr std::uint32_t ADC2_BASE             = APB2PERIPH_BASE + 0x2100;
constexpr std::uint32_t ADC3_BASE             = APB2PERIPH_BASE + 0x2200;
constexpr std::uint32_t ADC_BASE              = APB2PERIPH_BASE + 0x2300;
constexpr std::uint32_t SDMMC1_BASE           = APB2PERIPH_BASE + 0x2C00;
constexpr std::uint32_t SPI1_BASE             = APB2PERIPH_BASE + 0x3000;
constexpr std::uint32_t SPI4_BASE             = APB2PERIPH_BASE + 0x3400;
constexpr std::uint32_t SYSCFG_BASE           = APB2PERIPH_BASE + 0x3800;
constexpr std::uint32_t EXTI_BASE             = APB2PERIPH_BASE + 0x3C00;
constexpr std::uint32_t TIM9_BASE             = APB2PERIPH_BASE + 0x4000;
constexpr std::uint32_t TIM10_BASE            = APB2PERIPH_BASE + 0x4400;
constexpr std::uint32_t TIM11_BASE            = APB2PERIPH_BASE + 0x4800;
constexpr std::uint32_t SPI5_BASE             = APB2PERIPH_BASE + 0x5000;
constexpr std::uint32_t SPI6_BASE             = APB2PERIPH_BASE + 0x5400;
constexpr std::uint32_t SAI1_BASE             = APB2PERIPH_BASE + 0x5800;
constexpr std::uint32_t SAI2_BASE             = APB2PERIPH_BASE + 0x5C00;
constexpr std::uint32_t SAI1_Block_A_BASE     = SAI1_BASE + 0x004;
constexpr std::uint32_t SAI1_Block_B_BASE     = SAI1_BASE + 0x024;
constexpr std::uint32_t SAI2_Block_A_BASE     = SAI2_BASE + 0x004;
constexpr std::uint32_t SAI2_Block_B_BASE     = SAI2_BASE + 0x024;
constexpr std::uint32_t LTDC_BASE             = APB2PERIPH_BASE + 0x6800;
constexpr std::uint32_t LTDC_Layer1_BASE      = LTDC_BASE + 0x84;
constexpr std::uint32_t LTDC_Layer2_BASE      = LTDC_BASE + 0x104;
/*!< AHB1 peripherals */
constexpr std::uint32_t GPIOA_BASE            = AHB1PERIPH_BASE + 0x0000;
constexpr std::uint32_t GPIOB_BASE            = AHB1PERIPH_BASE + 0x0400;
constexpr std::uint32_t GPIOC_BASE            = AHB1PERIPH_BASE + 0x0800;
constexpr std::uint32_t GPIOD_BASE            = AHB1PERIPH_BASE + 0x0C00;
constexpr std::uint32_t GPIOE_BASE            = AHB1PERIPH_BASE + 0x1000;
constexpr std::uint32_t GPIOF_BASE            = AHB1PERIPH_BASE + 0x1400;
constexpr std::uint32_t GPIOG_BASE            = AHB1PERIPH_BASE + 0x1800;
constexpr std::uint32_t GPIOH_BASE            = AHB1PERIPH_BASE + 0x1C00;
constexpr std::uint32_t GPIOI_BASE            = AHB1PERIPH_BASE + 0x2000;
constexpr std::uint32_t GPIOJ_BASE            = AHB1PERIPH_BASE + 0x2400;
constexpr std::uint32_t GPIOK_BASE            = AHB1PERIPH_BASE + 0x2800;
constexpr std::uint32_t CRC_BASE              = AHB1PERIPH_BASE + 0x3000;
constexpr std::uint32_t RCC_BASE              = AHB1PERIPH_BASE + 0x3800;
constexpr std::uint32_t FLASH_R_BASE          = AHB1PERIPH_BASE + 0x3C00;
constexpr std::uint32_t DMA1_BASE             = AHB1PERIPH_BASE + 0x6000;
constexpr std::uint32_t DMA1_Stream0_BASE     = DMA1_BASE + 0x010;
constexpr std::uint32_t DMA1_Stream1_BASE     = DMA1_BASE + 0x028;
constexpr std::uint32_t DMA1_Stream2_BASE     = DMA1_BASE + 0x040;
constexpr std::uint32_t DMA1_Stream3_BASE     = DMA1_BASE + 0x058;
constexpr std::uint32_t DMA1_Stream4_BASE     = DMA1_BASE + 0x070;
constexpr std::uint32_t DMA1_Stream5_BASE     = DMA1_BASE + 0x088;
constexpr std::uint32_t DMA1_Stream6_BASE     = DMA1_BASE + 0x0A0;
constexpr std::uint32_t DMA1_Stream7_BASE     = DMA1_BASE + 0x0B8;
constexpr std::uint32_t DMA2_BASE             = AHB1PERIPH_BASE + 0x6400;
constexpr std::uint32_t DMA2_Stream0_BASE     = DMA2_BASE + 0x010;
constexpr std::uint32_t DMA2_Stream1_BASE     = DMA2_BASE + 0x028;
constexpr std::uint32_t DMA2_Stream2_BASE     = DMA2_BASE + 0x040;
constexpr std::uint32_t DMA2_Stream3_BASE     = DMA2_BASE + 0x058;
constexpr std::uint32_t DMA2_Stream4_BASE     = DMA2_BASE + 0x070;
constexpr std::uint32_t DMA2_Stream5_BASE     = DMA2_BASE + 0x088;
constexpr std::uint32_t DMA2_Stream6_BASE     = DMA2_BASE + 0x0A0;
constexpr std::uint32_t DMA2_Stream7_BASE     = DMA2_BASE + 0x0B8;
constexpr std::uint32_t ETH_BASE              = AHB1PERIPH_BASE + 0x8000;
constexpr std::uint32_t ETH_MAC_BASE          = ETH_BASE;
constexpr std::uint32_t ETH_MMC_BASE          = ETH_BASE + 0x0100;
constexpr std::uint32_t ETH_PTP_BASE          = ETH_BASE + 0x0700;
constexpr std::uint32_t ETH_DMA_BASE          = ETH_BASE + 0x1000;
constexpr std::uint32_t DMA2D_BASE            = AHB1PERIPH_BASE + 0xB000;
/*!< AHB2 peripherals */
constexpr std::uint32_t DCMI_BASE             = AHB2PERIPH_BASE + 0x50000;
constexpr std::uint32_t RNG_BASE              = AHB2PERIPH_BASE + 0x60800;
/*!< FMC Bankx registers base address */
constexpr std::uint32_t FMC_Bank1_R_BASE      = FMC_R_BASE + 0x0000;
constexpr std::uint32_t FMC_Bank1E_R_BASE     = FMC_R_BASE + 0x0104;
constexpr std::uint32_t FMC_Bank3_R_BASE      = FMC_R_BASE + 0x0080;
constexpr std::uint32_t FMC_Bank5_6_R_BASE    = FMC_R_BASE + 0x0140;

/* Debug MCU registers base address */
constexpr std::uint32_t DBGMCU_BASE           = 0xE0042000;

/*!< USB registers base address */
constexpr std::uint32_t USB_OTG_HS_PERIPH_BASE               = 0x40040000;
constexpr std::uint32_t USB_OTG_FS_PERIPH_BASE               = 0x50000000;

constexpr std::uint32_t USB_OTG_GLOBAL_BASE                  = 0x000;
constexpr std::uint32_t USB_OTG_DEVICE_BASE                  = 0x800;
constexpr std::uint32_t USB_OTG_IN_ENDPOINT_BASE             = 0x900;
constexpr std::uint32_t USB_OTG_OUT_ENDPOINT_BASE            = 0xB00;
constexpr std::uint32_t USB_OTG_EP_REG_SIZE                  = 0x20;
constexpr std::uint32_t USB_OTG_HOST_BASE                    = 0x400;
constexpr std::uint32_t USB_OTG_HOST_PORT_BASE               = 0x440;
constexpr std::uint32_t USB_OTG_HOST_CHANNEL_BASE            = 0x500;
constexpr std::uint32_t USB_OTG_HOST_CHANNEL_SIZE            = 0x20;
constexpr std::uint32_t USB_OTG_PCGCCTL_BASE                 = 0xE00;
constexpr std::uint32_t USB_OTG_FIFO_BASE                    = 0x1000;
constexpr std::uint32_t USB_OTG_FIFO_SIZE                    = 0x1000;

#endif // ADDR_H
