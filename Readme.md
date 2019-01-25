# stm32_bootloader
Bootloader For STM32F746 Discovery Board

Minimalist bootloader constructed in C++ without support libraries (so far, at least). It uses uart1 (Connected to STLink2.1) to transfer the data for the application.

The bootloader uses only the upper 120K of RAM memory and only one block of Flash (The first 32 kB block). This leaves 200 kB of available RAM for image loading, as well as 992kB of Flash.

Since it currently only boots from RAM the application always needs to be transfered right after booting.

Future plans:
  * Protect the bootloader using the MMC and setting the right ARM execution level before booting.
  * Add support for TFTP over Ethernet.
  * Protect firmware binaries from being extracted by a man-in-the-middle attack.
  * Sign firmware binaries to implement Secure Boot by having the bootloader as the Root of Trust.
  * Add support to update the bootloader itself.
