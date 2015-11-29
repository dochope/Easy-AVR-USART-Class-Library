#ifndef USART_HPP
#define USART_HPP

/************************************************************************************
 *  Published on: 13-02-2015                                                        *
 *  Author: jnk0le@hotmail.com                                                      *
 *  https://github.com/jnk0le                                                       *
 *  This library is distributed under MIT license terms                             *
 ************************************************************************************/

/* If you want more optimized for flash usage library, try out https://github.com/jnk0le/Easy-AVR-USART-C-Library */

// DO NOT DEFINE BUFFERS SIZES OR ANY SHARED MACROS IN 'main.cpp' CODE
// instead of this, define it in "Project Properties -> AVR C++ Compiler -> Symbols" or try to use -D gcc flag (eg. -DF_CPU=8000000)

//#define NO_USART_RX // disable all receiver code and dependencies 
//#define NO_USART_TX // disable all transmitter code and dependencies

//#define RX_STDIO_GETCHAR_ECHO // echoes back received characters in getchar() function (for reading in scanf())
#define RX_GETC_ECHO // echoes back received characters in getc() function

//#define PUTC_CONVERT_LF_TO_CRLF // allow for unix style (\n only) newline terminator in stored strings // not included into putc_noblock
#define RX_NEWLINE_MODE 2 // 0 - \r,  1 - \n,  2 - /r/n
// lot of terminals sends only \r character as a newline terminator, instead of \r\n or even unix style \n 
// (BTW PuTTY doesn't allow to change this) but in return requires \r\n terminator to show not broken text

//#define USE_DOUBLE_SPEED // DEPRECATED // enables double speed for all available USART interfaces 

//#define USART_USE_SEPARATED_BUFFERS // coming soon

/*****************************multiple usart config*************************************/

//#define NO_USART0 // disable usage of uart0
//#define NO_USART1 // disable usage of uart1
//#define NO_USART2 // disable usage of uart2 
//#define NO_USART3 // disable usage of uart3

//#define NO_RX0_INTERRUPT // disables interrupt handling and frees RX0 gpio port // combining with NO_USART_RX is not necessary
//#define NO_RX1_INTERRUPT // disables interrupt handling and frees RX1 gpio port // combining with NO_USART_RX is not necessary
//#define NO_RX2_INTERRUPT // disables interrupt handling and frees RX2 gpio port // combining with NO_USART_RX is not necessary
//#define NO_RX3_INTERRUPT // disables interrupt handling and frees RX3 gpio port // combining with NO_USART_RX is not necessary

//#define NO_TX0_INTERRUPT // disables interrupt handling and frees TX0 gpio port // combining with NO_USART_TX is not necessary
//#define NO_TX1_INTERRUPT // disables interrupt handling and frees TX1 gpio port // combining with NO_USART_TX is not necessary
//#define NO_TX2_INTERRUPT // disables interrupt handling and frees TX2 gpio port // combining with NO_USART_TX is not necessary
//#define NO_TX3_INTERRUPT // disables interrupt handling and frees TX3 gpio port // combining with NO_USART_TX is not necessary

//#define USART0_U2X_SPEED // DEPRECATED // enables double speed for USART0 // combining with USE_DOUBLE_SPEED is not necessary
//#define USART1_U2X_SPEED // DEPRECATED // enables double speed for USART1 // combining with USE_DOUBLE_SPEED is not necessary
//#define USART2_U2X_SPEED // DEPRECATED // enables double speed for USART2 // combining with USE_DOUBLE_SPEED is not necessary
//#define USART3_U2X_SPEED // DEPRECATED // enables double speed for USART3 // combining with USE_DOUBLE_SPEED is not necessary

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#ifndef F_CPU
	#error F_CPU is undefined, USART cannot work correctly without this parametr
#endif

#define BAUD_CALC(x) ((F_CPU+(x)*8UL) / (16UL*(x))-1UL) // macro calculating UBBR value
#define DOUBLE_BAUD_CALC(x) ((F_CPU+(x)*4UL) / (8UL*(x))-1UL) // macro calculating UBBR value for double speed

#ifndef __OPTIMIZE__
	#warning "Compiler optimizations disabled; functions from usart.h won't work as designed"
	#define USART_DO_NOT_INLINE
#endif

#ifdef DEBUG
	#define USART_DO_NOT_INLINE
#endif

#ifndef RX_BUFFER_SIZE
	#define RX_BUFFER_SIZE 32 // Size of the ring buffers, must be power of 2
#endif

#ifndef TX_BUFFER_SIZE
	#define TX_BUFFER_SIZE 32 // Size of the ring buffers, must be power of 2
#endif

#ifndef TX0_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define TX0_BUFFER_SIZE TX_BUFFER_SIZE
#endif
#ifndef RX0_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define RX0_BUFFER_SIZE RX_BUFFER_SIZE
#endif
#ifndef TX1_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define TX1_BUFFER_SIZE TX_BUFFER_SIZE
#endif
#ifndef RX1_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define RX1_BUFFER_SIZE RX_BUFFER_SIZE
#endif
#ifndef TX2_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define TX2_BUFFER_SIZE TX_BUFFER_SIZE
#endif
#ifndef RX2_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define RX2_BUFFER_SIZE RX_BUFFER_SIZE
#endif
#ifndef TX3_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define TX3_BUFFER_SIZE TX_BUFFER_SIZE
#endif
#ifndef RX3_BUFFER_SIZE // Size of the ring buffers, must be power of 2
	#define RX3_BUFFER_SIZE RX_BUFFER_SIZE
#endif

#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)// ... only before replacing in functions
#define TX_BUFFER_MASK (TX_BUFFER_SIZE - 1)// ... only before replacing in functions

#define TX0_BUFFER_MASK (TX0_BUFFER_SIZE - 1)
#define RX0_BUFFER_MASK (RX0_BUFFER_SIZE - 1)

#define TX1_BUFFER_MASK (TX1_BUFFER_SIZE - 1)
#define RX1_BUFFER_MASK (RX1_BUFFER_SIZE - 1)

#define TX2_BUFFER_MASK (TX2_BUFFER_SIZE - 1)
#define RX2_BUFFER_MASK (RX2_BUFFER_SIZE - 1)

#define TX3_BUFFER_MASK (TX3_BUFFER_SIZE - 1)
#define RX3_BUFFER_MASK (RX3_BUFFER_SIZE - 1)

enum {COMPLETED = 1, BUFFER_EMPTY = 0, BUFFER_FULL = 0};	

#ifdef NO_USART_RX // remove all RX interrupts
	#define NO_RX0_INTERRUPT
	#define NO_RX1_INTERRUPT
	#define NO_RX2_INTERRUPT
	#define NO_RX3_INTERRUPT
#endif

#ifdef NO_USART_TX // remove all TX interrupts
	#define NO_TX0_INTERRUPT
	#define NO_TX1_INTERRUPT
	#define NO_TX2_INTERRUPT
	#define NO_TX3_INTERRUPT
#endif

#ifdef USE_DOUBLE_SPEED
	#define USART0_U2X_SPEED
	#define USART1_U2X_SPEED
	#define USART2_U2X_SPEED
	#define USART3_U2X_SPEED
#endif

#ifdef RX_NEWLINE_MODE
	
	#if (RX_NEWLINE_MODE == 0)
		#define RX_NEWLINE_MODE_R
	#elif (RX_NEWLINE_MODE == 1)
		#define RX_NEWLINE_MODE_N
	#else // RX_NEWLINE_MODE == 2
		#define RX_NEWLINE_MODE_RN
	#endif
#else
	#define RX_NEWLINE_MODE_RN // 2
#endif

#if defined(__AVR_ATtiny2313__)||defined(__AVR_ATtiny2313A__)||defined(__AVR_ATtiny4313)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART_RX_vect
	//#define TX0_INTERRUPT		USART_TX_vect
	#define TX0_INTERRUPT		USART_UDRE_vect
	#define UDR0_REGISTER		UDR
	#define UBRR0L_REGISTER		UBRRL
	#define UBRR0H_REGISTER		UBRRH
	#define UCSR0A_REGISTER		UCSRA
	#define UCSR0B_REGISTER		UCSRB
	#define UCSR0C_REGISTER		UCSRC
	//#define TXCIE0_BIT  		TXCIE
	#define UDRIE0_BIT    		UDRIE
	#define RXCIE0_BIT  		RXCIE
	#define TXEN0_BIT   		TXEN
	#define RXEN0_BIT   		RXEN
	#define U2X0_BIT    		U2X

#endif //NO_USART0
#endif

#if defined(__AVR_ATmega48__)||defined(__AVR_ATmega48P__)||defined(__AVR_ATmega48PA__)\
||defined(__AVR_ATmega88__)||defined(__AVR_ATmega88P__)||defined(__AVR_ATmega88PA__)\
||defined(__AVR_ATmega168__)||defined(__AVR_ATmega168P__)||defined(__AVR_ATmega168PA__)\
||defined(__AVR_ATmega328__)||defined(__AVR_ATmega328P__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART_RX_vect
	//#define TX0_INTERRUPT		USART_TX_vect
	#define TX0_INTERRUPT		USART_UDRE_vect
	#define UDR0_REGISTER		UDR0
	#define UBRR0L_REGISTER		UBRR0L
	#define UBRR0H_REGISTER		UBRR0H
	#define UCSR0A_REGISTER		UCSR0A
	#define UCSR0B_REGISTER		UCSR0B
	#define UCSR0C_REGISTER		UCSR0C
	//#define TXCIE0_BIT  		TXCIE0
	#define UDRIE0_BIT    		UDRIE0
	#define RXCIE0_BIT   		RXCIE0
	#define TXEN0_BIT   		TXEN0
	#define RXEN0_BIT   		RXEN0
	#define U2X0_BIT    		U2X0

#endif //NO_USART0
#endif

#if defined(__AVR_ATmega8__)||defined(__AVR_ATmega8P__)||defined(__AVR_ATmega16__)\
||defined(__AVR_ATmega16A__)||defined(__AVR_ATmega32__)||defined(__AVR_ATmega32A__)\
||defined(__AVR_ATmega8A__)||defined(__AVR_ATmega8L__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART_RXC_vect
	//#define TX0_INTERRUPT		USART_TXC_vect
	#define TX0_INTERRUPT		USART_UDRE_vect
	#define UDR0_REGISTER		UDR
	#define UBRR0L_REGISTER		UBRRL
	#define UBRR0H_REGISTER		UBRRH
	#define UCSR0A_REGISTER		UCSRA
	#define UCSR0B_REGISTER		UCSRB
	#define UCSR0C_REGISTER		UCSRC
	//#define TXCIE0_BIT  		TXCIE
	#define UDRIE0_BIT    		UDRIE
	#define RXCIE0_BIT  		RXCIE
	#define TXEN0_BIT   		TXEN
	#define RXEN0_BIT   		RXEN
	#define U2X0_BIT    		U2X

#endif //NO_USART0
#endif

#if defined(__AVR_ATmega8515__)||defined(__AVR_ATmega8515L__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART_RX_vect
	//#define TX0_INTERRUPT		USART_TX_vect
	#define TX0_INTERRUPT		USART_UDRE_vect
	#define UDR0_REGISTER		UDR
	#define UBRR0L_REGISTER		UBRRL
	#define UBRR0H_REGISTER		UBRRH
	#define UCSR0A_REGISTER		UCSRA
	#define UCSR0B_REGISTER		UCSRB
	#define UCSR0C_REGISTER		UCSRC
	//#define TXCIE0_BIT  		TXCIE
	#define UDRIE0_BIT    		UDRIE
	#define RXCIE0_BIT  		RXCIE
	#define TXEN0_BIT   		TXEN
	#define RXEN0_BIT   		RXEN
	#define U2X0_BIT    		U2X

#endif //NO_USART0
#endif

#if defined(__AVR_ATmega162__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART0_RXC_vect
	//#define TX0_INTERRUPT		USART0_TXC_vect
	#define TX0_INTERRUPT		USART0_UDRE_vect
	#define UDR0_REGISTER		UDR0
	#define UBRR0L_REGISTER		UBRR0L
	#define UBRR0H_REGISTER		UBRR0H
	#define UCSR0A_REGISTER		UCSR0A
	#define UCSR0B_REGISTER		UCSR0B
	#define UCSR0C_REGISTER		UCSR0C
	//#define TXCIE0_BIT  		TXCIE0
	#define UDRIE0_BIT    		UDRIE0
	#define RXCIE0_BIT  		RXCIE0
	#define TXEN0_BIT   		TXEN0
	#define RXEN0_BIT   		RXEN0
	#define U2X0_BIT    		U2X0

#endif //NO_USART0

#ifndef NO_USART1
#define USE_USART1

	#define RX1_INTERRUPT		USART1_RXC_vect
	//#define TX1_INTERRUPT		USART1_TXC_vect
	#define TX1_INTERRUPT		USART1_UDRE_vect
	#define UDR1_REGISTER		UDR1
	#define UBRR1L_REGISTER		UBRR1L
	#define UBRR1H_REGISTER		UBRR1H
	#define UCSR1A_REGISTER		UCSR1A
	#define UCSR1B_REGISTER		UCSR1B
	#define UCSR1C_REGISTER		UCSR1C
	//#define TXCIE1_BIT  		TXCIE1
	#define UDRIE1_BIT    		UDRIE1
	#define RXCIE1_BIT  		RXCIE1
	#define TXEN1_BIT   		TXEN1
	#define RXEN1_BIT   		RXEN1
	#define U2X1_BIT    		U2X1

#endif //NO_USART1
#endif

#if defined(__AVR_ATmega644__)||defined(__AVR_ATmega644P__)||defined(__AVR_ATmega644PA__)\
||defined(__AVR_ATmega1284__)||defined(__AVR_ATmega1284P__)||defined(__AVR_ATmega128__)\
||defined(__AVR_ATmega128A__)||defined(__AVR_ATmega64__)||defined(__AVR_ATmega64A__)\
||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)||defined(__AVR_ATmega640__)\
||defined(__AVR_ATmega1280__)||defined(__AVR_ATmega2560__)||defined(__AVR_ATmega164P__)\
||defined(__AVR_ATmega324P__)||defined(__AVR_ATmega324A__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART0_RX_vect
	//#define TX0_INTERRUPT		USART0_TX_vect
	#define TX0_INTERRUPT		USART0_UDRE_vect
	#define UDR0_REGISTER		UDR0
	#define UBRR0L_REGISTER		UBRR0L
	#define UBRR0H_REGISTER		UBRR0H
	#define UCSR0A_REGISTER		UCSR0A
	#define UCSR0B_REGISTER		UCSR0B
	#define UCSR0C_REGISTER		UCSR0C
	//#define TXCIE0_BIT  		TXCIE0
	#define UDRIE0_BIT    		UDRIE0
	#define RXCIE0_BIT  		RXCIE0
	#define TXEN0_BIT   		TXEN0
	#define RXEN0_BIT   		RXEN0
	#define U2X0_BIT    		U2X0

#endif // NO_USART2

#if !defined(NO_USART1) && !defined(__AVR_ATmega644__)
#define USE_USART1

	#define RX1_INTERRUPT		USART1_RX_vect
	//#define TX1_INTERRUPT		USART1_TX_vect
	#define TX1_INTERRUPT		USART1_UDRE_vect
	#define UDR1_REGISTER		UDR1
	#define UBRR1L_REGISTER		UBRR1L
	#define UBRR1H_REGISTER		UBRR1H
	#define UCSR1A_REGISTER		UCSR1A
	#define UCSR1B_REGISTER		UCSR1B
	#define UCSR1C_REGISTER		UCSR1C
	//#define TXCIE1_BIT  		TXCIE1
	#define UDRIE1_BIT    		UDRIE1
	#define RXCIE1_BIT  		RXCIE1
	#define TXEN1_BIT   		TXEN1
	#define RXEN1_BIT   		RXEN1
	#define U2X1_BIT    		U2X1

#endif // NO_USART1 && 644
#endif // mcu

#if defined(__AVR_ATmega640__)||defined(__AVR_ATmega1280__)||defined(__AVR_ATmega2560__)

#ifndef NO_USART2
#define USE_USART2

	#define RX2_INTERRUPT		USART2_RX_vect
	//#define TX2_INTERRUPT		USART2_TX_vect
	#define TX2_INTERRUPT		USART2_UDRE_vect
	#define UDR2_REGISTER		UDR2
	#define UBRR2L_REGISTER		UBRR2L
	#define UBRR2H_REGISTER		UBRR2H
	#define UCSR2A_REGISTER		UCSR2A
	#define UCSR2B_REGISTER		UCSR2B
	#define UCSR2C_REGISTER		UCSR2C
	//#define TXCIE2_BIT  		TXCIE2
	#define UDRIE2_BIT    		UDRIE2
	#define RXCIE2_BIT  		RXCIE2
	#define TXEN2_BIT   		TXEN2
	#define RXEN2_BIT   		RXEN2
	#define U2X2_BIT    		U2X2

#endif // NO_USART2

#ifndef NO_USART3
#define USE_USART3

	#define RX3_INTERRUPT		USART3_RX_vect
	//#define TX3_INTERRUPT		USART3_TX_vect
	#define TX3_INTERRUPT		USART3_UDRE_vect
	#define UDR3_REGISTER		UDR3
	#define UBRR3L_REGISTER 	UBRR3L
	#define UBRR3H_REGISTER		UBRR3H
	#define UCSR3A_REGISTER		UCSR3A
	#define UCSR3B_REGISTER		UCSR3B
	#define UCSR3C_REGISTER		UCSR3C
	//#define TXCIE3_BIT  		TXCIE3
	#define UDRIE3_BIT    		UDRIE3
	#define RXCIE3_BIT  		RXCIE3
	#define TXEN3_BIT   		TXEN3
	#define RXEN3_BIT   		RXEN3
	#define U2X3_BIT    		U2X3

#endif // NO_USART3

#endif // 640/1280/2560

#if defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega16U4__)\
||defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U6__)

#ifndef NO_USART0 // we will call the only usart, as an usart0
#define USE_USART0

	#define RX0_INTERRUPT		USART1_RX_vect
	//#define TX0_INTERRUPT		USART1_TX_vect
	#define TX0_INTERRUPT		USART1_UDRE_vect
	#define UDR0_REGISTER		UDR1
	#define UBRR0L_REGISTER		UBRR1L
	#define UBRR0H_REGISTER		UBRR1H
	#define UCSR0A_REGISTER		UCSR1A
	#define UCSR0B_REGISTER		UCSR1B
	#define UCSR0C_REGISTER		UCSR1C
	//#define TXCIE0_BIT  		TXCIE1
	#define UDRIE0_BIT    		UDRIE1
	#define RXCIE0_BIT  		RXCIE1
	#define TXEN0_BIT   		TXEN1
	#define RXEN0_BIT   		RXEN1
	#define U2X0_BIT    		U2X1

#endif // NO_USART0
#endif

#if defined(__AVR_ATmega169A__)||defined(__AVR_ATmega169__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART0_RX_vect
	//#define TX0_INTERRUPT		USART0_TX_vect
	#define TX0_INTERRUPT		USART0_UDRE_vect
	#define UDR0_REGISTER		UDR
	#define UBRR0L_REGISTER		UBRRL
	#define UBRR0H_REGISTER		UBRRH
	#define UCSR0A_REGISTER		UCSRA
	#define UCSR0B_REGISTER		UCSRB
	#define UCSR0C_REGISTER		UCSRC
	//#define TXCIE0_BIT  		TXCIE
	#define UDRIE0_BIT    		UDRIE
	#define RXCIE0_BIT  		RXCIE
	#define TXEN0_BIT   		TXEN
	#define RXEN0_BIT   		RXEN
	#define U2X0_BIT    		U2X

#endif //NO_USART0
#endif


#if defined(__AVR_ATmega329__)||defined(__AVR_ATmega329P__)||defined(__AVR_ATmega329PA__)\
||defined(__AVR_ATmega329A__)||defined(__AVR_ATmega649__)||defined(__AVR_ATmega649A__)\
||defined(__AVR_ATmega649P__)||defined(__AVR_ATmega169P__)||defined(__AVR_ATmega169PA__)\
||defined(__AVR_ATmega325__)||defined(__AVR_ATmega325A__)||defined(__AVR_ATmega325P__)\
||defined(__AVR_ATmega325PA__)||defined(__AVR_ATmega645__)||defined(__AVR_ATmega645A__)\
||defined(__AVR_ATmega645P__)


#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART0_RX_vect
	//#define TX0_INTERRUPT		USART0_TX_vect
	#define TX0_INTERRUPT		USART0_UDRE_vect
	#define UDR0_REGISTER		UDR0
	#define UBRR0L_REGISTER		UBRR0L
	#define UBRR0H_REGISTER		UBRR0H
	#define UCSR0A_REGISTER		UCSR0A
	#define UCSR0B_REGISTER		UCSR0B
	#define UCSR0C_REGISTER		UCSR0C
	//#define TXCIE0_BIT  		TXCIE0
	#define UDRIE0_BIT    		UDRIE0
	#define RXCIE0_BIT  		RXCIE0
	#define TXEN0_BIT   		TXEN0
	#define RXEN0_BIT   		RXEN0
	#define U2X0_BIT    		U2X0

#endif //NO_USART0
#endif

#if defined(__AVR_ATmega3290__)||defined(__AVR_ATmega6490__)||defined(__AVR_ATmega3290P__)\
||defined(__AVR_ATmega3290PA__)||defined(__AVR_ATmega3290A__)||defined(__AVR_ATmega6490A__)\
||defined(__AVR_ATmega6490P__)||defined(__AVR_ATmega3250__)||defined(__AVR_ATmega3250A__)\
||defined(__AVR_ATmega3250P__)||defined(__AVR_ATmega3250PA__)||defined(__AVR_ATmega6450__)\
||defined(__AVR_ATmega6450A__)||defined(__AVR_ATmega6450P__)

#ifndef NO_USART0
#define USE_USART0

	#define RX0_INTERRUPT		USART_RX_vect // wtf
	//#define TX0_INTERRUPT		USART0_TX_vect
	#define TX0_INTERRUPT		USART0_UDRE_vect
	#define UDR0_REGISTER		UDR0
	#define UBRR0L_REGISTER		UBRR0L
	#define UBRR0H_REGISTER		UBRR0H
	#define UCSR0A_REGISTER		UCSR0A
	#define UCSR0B_REGISTER		UCSR0B
	#define UCSR0C_REGISTER		UCSR0C
	//#define TXCIE0_BIT  		TXCIE0
	#define UDRIE0_BIT    		UDRIE0
	#define RXCIE0_BIT  		RXCIE0
	#define TXEN0_BIT   		TXEN0
	#define RXEN0_BIT   		RXEN0
	#define U2X0_BIT    		U2X0

#endif //NO_USART0
#endif

#if !defined(USE_USART0) && !defined(USE_USART1) && !defined(USE_USART2) && !defined(USE_USART3)
	#error USART not available or unknown mcu
#endif

#ifndef USE_USART0
	#define NO_TX0_INTERRUPT
	#define NO_RX0_INTERRUPT
#endif

#ifndef USE_USART1
	#define NO_TX1_INTERRUPT
	#define NO_RX1_INTERRUPT
#endif

#ifndef USE_USART2
	#define NO_TX2_INTERRUPT
	#define NO_RX2_INTERRUPT
#endif

#ifndef USE_USART3
	#define NO_TX3_INTERRUPT
	#define NO_RX3_INTERRUPT
#endif

#ifndef USART0_CONFIG_B // set config bytes for UCSR0B_REGISTER
	
	#if defined(NO_RX0_INTERRUPT)
		#define USART0_CONFIG_B (1<<TXEN0_BIT)
	
	#elif defined(NO_TX0_INTERRUPT)
		#define USART0_CONFIG_B (1<<RXEN0_BIT)|(1<<RXCIE0_BIT)
	#else
		#define USART0_CONFIG_B (1<<TXEN0_BIT)|(1<<RXEN0_BIT)|(1<<RXCIE0_BIT)
	#endif 
#endif // USART0_CONFIG

#ifndef USART1_CONFIG_B // set config bytes for UCSR1B_REGISTER
	
	#if defined(NO_RX1_INTERRUPT)
		#define USART1_CONFIG_B (1<<TXEN1_BIT)

	#elif defined(NO_TX1_INTERRUPT)
		#define USART1_CONFIG_B (1<<RXEN1_BIT)|(1<<RXCIE1_BIT)
	#else
		#define USART1_CONFIG_B (1<<TXEN1_BIT)|(1<<RXEN1_BIT)|(1<<RXCIE1_BIT)
	#endif
#endif // USART1_CONFIG

#ifndef USART2_CONFIG_B // set config bytes for UCSR2B_REGISTER

	#if defined(NO_RX2_INTERRUPT)
		#define USART2_CONFIG_B (1<<TXEN2_BIT)

	#elif defined(NO_TX2_INTERRUPT)
		#define USART2_CONFIG_B (1<<RXEN2_BIT)|(1<<RXCIE2_BIT)
	#else
		#define USART2_CONFIG_B (1<<TXEN2_BIT)|(1<<RXEN2_BIT)|(1<<RXCIE2_BIT)
	#endif
#endif // USART2_CONFIG

#ifndef USART3_CONFIG_B // set config bytes for UCSR3B_REGISTER

	#if defined(NO_RX3_INTERRUPT)
		#define USART3_CONFIG_B (1<<TXEN3_BIT)

	#elif defined(NO_TX3_INTERRUPT)
		#define USART3_CONFIG_B (1<<RXEN3_BIT)|(1<<RXCIE3_BIT)
	#else
		#define USART3_CONFIG_B (1<<TXEN3_BIT)|(1<<RXEN3_BIT)|(1<<RXCIE3_BIT)
	#endif
#endif // USART3_CONFIG

class USART
{
public: // house

#ifndef NO_USART_TX
	volatile uint8_t tx_first_byte, tx_last_byte;
	char tx_buffer[TX_BUFFER_SIZE];
#endif

#ifndef NO_USART_RX
	volatile uint8_t rx_first_byte, rx_last_byte;
	char rx_buffer[RX_BUFFER_SIZE];
#endif

#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
	uint8_t usartct;
#endif

/************************************************************************************
 *                            Initializers                                          *
 ************************************************************************************/
#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
	void init(uint16_t ubbr_value = BAUD_CALC(9600), uint8_t usartcnter = 0);
	void begin(uint16_t ubbr_value = BAUD_CALC(9600), uint8_t usartcnter = 0) { this -> init(ubbr_value, usartcnter); }
#else
	void init(uint16_t ubbr_value = BAUD_CALC(9600));
	void begin(uint16_t ubbr_value = BAUD_CALC(9600)) { this -> init(ubbr_value); }
#endif
	
	void set_UCSRC(uint8_t UCSRC_reg);
	// UCSRC_reg can be used to set other than 8n1 transmission
	void set_U2X(void); // function instead of macro

/************************************************************************************
 *                          Transmitter functions                                   *
 ************************************************************************************/
#ifndef NO_USART_TX
	
	void putc(char data); // put character/data into transmitter ring buffer
	uint8_t putc_noblock(char data); // returns BUFFER_FULL (false) if buffer is full and character cannot be sent at the moment
	
	void putstr(char *string, uint8_t BytesToWrite); // in case of bascom users or buffers without NULL byte ending (binary transmission allowed)
	void putstr(char *string); // send string from the array buffer 
	// stops when NULL byte is hit (NULL byte is not included into transmission)
	
	void puts(const char* str) { this -> putstr(const_cast<char*>(str)); }
	//#define puts(str) putstr(const_cast<char*>(str)) // macro to avoid const char* conversion restrictions
	
	void puts_p(const char *string); // send string from flash memory 
		#define puts_P(__s)    puts_p(PSTR(__s)) // macro to automatically put a string constant into flash
		
	void putint(int16_t data);
	void putint(int16_t data, uint8_t radix);
	
	void putint(uint16_t data) { this -> putuint(data); }
	void putint(uint16_t data, uint8_t radix) { this -> putuint(data, radix); }
	
	void putuint(uint16_t data);
	void putuint(uint16_t data, uint8_t radix);
	
	void putint(int32_t data) { this -> putlong(data); }
	void putint(int32_t data, uint8_t radix) { this -> putlong(data, radix); }
		
	void putint(uint32_t data) { this -> putulong(data); }
	void putint(uint32_t data, uint8_t radix) { this -> putulong(data, radix); }
	
	void putlong(int32_t data);
	void putlong(int32_t data, uint8_t radix);
	
	void putlong(uint32_t data){ this -> putulong(data); }
	void putlong(uint32_t data, uint8_t radix){ this -> putulong(data, radix); }

	void putulong(uint32_t data);
	void putulong(uint32_t data, uint8_t radix);
	
	void put_hex(uint16_t data);
	
	void putfloat(float data);
	void putfloat(float data, uint8_t precision);
	
	USART& operator<<(char &c)
	{
		this -> putc(c);
		return *this;
	}
	
	USART& operator<<(uint8_t dat)
	{
		this -> put_hex(dat);
		return *this;
	}
	
	USART& operator<<(const char* str)
	{
		this -> puts(str);
		return *this;
	}
	
	USART& operator<<(char* str)
	{
		this -> putstr(str);
		return *this;
	}
	
	USART& operator<<(int16_t num)
	{
		this -> putint(num);
		return *this;
	}
	
	USART& operator<<(uint16_t num)
	{
		this -> putint(num);
		return *this;
	}
	
	USART& operator<<(int32_t num)
	{
		this -> putlong(num);
		return *this;
	}
	
	USART& operator<<(uint32_t num)
	{
		this -> putlong(num);
		return *this;
	}

	USART& operator<<(double num)
	{
		this -> putfloat(num);
		return *this;
	}
	
#endif // NO_USART_TX
/************************************************************************************
 *                           Receiver functions                                     *
 ************************************************************************************/
#ifndef NO_USART_RX

	char getc(void); // get character from receiver ring buffer
	
	void gets(char *buffer); // DEPRECATED - possibility of buffer overflows
	void gets(char *buffer, uint8_t bufferlimit); // reads whole receiver buffer or bufferlimit-1 characters
	// newline terminator will not be cut // adds NULL byte at the end of string
	void getln(char *buffer, uint8_t bufferlimit); // reads one line from the buffer
	// waits for newline terminator or reached bufferlimit // adds NULL byte at the end of string
	
	void getlnToFirstWhiteSpace(char *buffer, uint8_t bufferlimit); // read one line to the first whitescape after the string
	// cuts all whitespaces before string and one after the string
	
	char skipWhiteSpaces(void); // returns first nonspace character found in the buffer
	
	int16_t getint(void);
	int32_t getlong(void);
	float getfloat(void);
	
	uint8_t getData(uint8_t &data) { return this -> getData(&data); }
	uint8_t getData(uint8_t *data); // reads binary data from a buffer and loads it into &data byte
	// in case of empty buffers returned flag is set to BUFFER_EMPTY - NULL
	
	uint8_t AvailableBytes(void); // returns number of bytes waiting in the receiver buffer
	uint8_t Available(void) { return this -> AvailableBytes(); } 
	
	USART& operator>>(char& c)
	{
		while( !(c = this -> getc()) );
		return *this;
	}
	
	USART& operator>>(uint8_t& c)
	{
		while( !(c = this -> getc()) );
		return *this;
	}

	USART& operator>>(char* buff)
	{
		this -> getln(buff, 255); // not safe reading by now
		return *this;
	}
	
	USART& operator>>(int16_t& num)
	{
		num = this -> getint();
		return *this;
	}
	
	USART& operator>>(int32_t& num)
	{
		num = this -> getlong();
		return *this;
	}
		
	USART& operator>>(double& num)
	{
		num = this -> getfloat();
		return *this;
	}
	
	USART& operator>>(float& num)
	{
		num = this -> getfloat();
		return *this;
	}
	
#endif // NO_USART_RX
};

#endif // USART_HPP