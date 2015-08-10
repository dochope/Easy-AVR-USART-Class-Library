# Easy-AVR-USART-Class-Library
An interrupt-driven USART (RS232) library for AVR microcontrollers, with support for multiple UARTs, using ring
buffers for receive/transmit. Designed to be easy to use, especially like arduino libraries (with less overload).

# Features
- easy to use
- support for up to 4 USART's
- allow for binary transmission
- optimized as much as possible to reduce code size
- cin/cout like streams (<< and >>)
- and much more

simple "hello world" gives

         text      data       bss       dec       hex   filename
          474        16        70       560       230   avrtpp.elf
   
Program Memory Usage    :   490 bytes   1,5 % Full
Data Memory Usage       :   86 bytes   4,2 % Full

defined  NO_USART_RX flag gives

         text      data       bss       dec       hex   filename
          390        16        36       442       1ba   avrtpp.elf

Program Memory Usage    :   406 bytes   1,2 % Full
Data Memory Usage       :   52 bytes   2,5 % Full




