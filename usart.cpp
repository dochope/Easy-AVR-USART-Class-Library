//**************************************************************
// ****** FUNCTIONS FOR USART COMMUNICATION *******
//**************************************************************
//Compiler          : AVR-GCC
//Author            : jnk0le@hotmail.com
//                    https://github.com/jnk0le
//Date              : 13 February 2015
//License           : MIT
//**************************************************************

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.hpp"

#ifdef USE_USART0
	USART *pUSART0;
#endif
#ifdef USE_USART1
	USART *pUSART1;
#endif
#ifdef USE_USART2
	USART *pUSART2;
#endif
#ifdef USE_USART3
	USART *pUSART3;
#endif

#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
	
	void USART::init(uint16_t ubbr_value, uint8_t usartcnter)
	{
		usartct = usartcnter;
		switch(usartcnter)
		{
			default: // first found case as default (byte saving)
		#ifdef USE_USART1
			case 0:
				UBRR0L_REGISTER = (uint8_t) ubbr_value;
				UBRR0H_REGISTER = (ubbr_value>>8);
		
			#ifdef USART0_U2X_SPEED
				UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
			#endif
				UCSR0B_REGISTER = USART0_CONFIG_B; 
				// (1<<RXCIE0_BIT)|(1<<TXEN0_BIT)|(1<<RXEN0_BIT); 
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART0 = this; // IS SPARTA
				break;
		#endif // USE_USART0
		#ifdef USE_USART1
			case 1:
				UBRR1L_REGISTER = (uint8_t) ubbr_value;
				UBRR1H_REGISTER = (ubbr_value>>8);
		
			#ifdef USART1_U2X_SPEED
				UCSR1A_REGISTER |= (1<<U2X1_BIT); // enable double speed
			#endif
				UCSR1B_REGISTER = USART1_CONFIG_B;
				// (1<<RXCIE1_BIT)|(1<<TXEN1_BIT)|(1<<RXEN1_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART1 = this; // IS SPARTA
				break;
			
		#endif // USE_USART1
		#ifdef USE_USART2
			case 2:
				UBRR2L_REGISTER = (uint8_t) ubbr_value;
				UBRR2H_REGISTER = (ubbr_value>>8);
		
			#ifdef USART2_U2X_SPEED
				UCSR2A_REGISTER |= (1<<U2X2_BIT); // enable double speed
			#endif
				UCSR2B_REGISTER = USART2_CONFIG_B;
				// (1<<RXCIE2_BIT)|(1<<TXEN2_BIT)|(1<<RXEN2_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART2 = this; // IS SPARTA
				break;
		
		#endif // USE_USART2
		#ifdef USE_USART3
			case 3:
				UBRR3L_REGISTER = (uint8_t) ubbr_value;
				UBRR3H_REGISTER = (ubbr_value>>8);
		
			#ifdef USART3_U2X_SPEED
				UCSR3A_REGISTER |= (1<<U2X3_BIT); // enable double speed
			#endif
				UCSR3B_REGISTER = USART3_CONFIG_B;
				// (1<<RXCIE3_BIT)|(1<<TXEN3_BIT)|(1<<RXEN3_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART3 = this; // IS SPARTA
				break;
		#endif // USE_USART3
		}
	
	}

#else // no USART1/2/3
	
	void USART::init(uint16_t ubbr_value)
	{
		UBRR0L_REGISTER = (uint8_t) ubbr_value;
		UBRR0H_REGISTER = (ubbr_value>>8);
	
	#ifdef USART0_U2X_SPEED
		UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
	#endif
		UCSR0B_REGISTER = USART0_CONFIG_B; 
		// (1<<TXEN0_BIT)|(1<<RXEN0_BIT)|(1<<RXCIE0_BIT); 
		// 8n1 is set by default, setting UCSRC is not needed 
		pUSART0 = this; 
	
	}

#endif //USE_USART1/2/3

#ifndef NO_USART_TX

//******************************************************************
//Function  : Send single character/byte.
//Arguments : Character/byte to send.
//Return    : none
//******************************************************************
	void USART::putc(char data)
	{
	#ifdef PUTC_CONVERT_LF_TO_CRLF
		if(data == '\n')
			this -> putc(usartct, '\r');
	#endif
		register uint8_t tmp_tx_last_byte = (tx_last_byte + 1) & TX_BUFFER_MASK;
		
		while(tx_first_byte == tmp_tx_last_byte); // wait for free space in buffer
		
		tx_buffer[tmp_tx_last_byte] = data;
		tx_last_byte = tmp_tx_last_byte;
		
	#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
		switch(usartct)
		{
			default: // first found case as default (byte saving)
		#ifndef NO_TX0_INTERRUPT
			case 0:
				UCSR0B_REGISTER |= (1<<UDRIE0_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX0_INTERRUPT
		#ifndef NO_TX1_INTERRUPT
			case 1:
				UCSR1B_REGISTER |= (1<<UDRIE1_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX1_INTERRUPT
		#ifndef NO_TX2_INTERRUPT
			case 2:
				UCSR2B_REGISTER |= (1<<UDRIE2_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX2_INTERRUPT
		#ifndef NO_TX3_INTERRUPT
			case 3:
				UCSR3B_REGISTER |= (1<<UDRIE3_BIT); // enable UDRE interrupt
			//break;
		#endif // NO_TX3_INTERRUPT
		}
	#else
		UCSR0B_REGISTER |= (1<<UDRIE0_BIT); // enable UDRE interrupt
	#endif
		
	}
	
//******************************************************************
//Function  : Send single character/byte.
//Arguments : Character/byte to send.
//Return    : Status value: 0 = BUFFER_FULL, 1 = COMPLETED.
//Note      : If character cannot be sent due to full transmit buffer, function will abort transmitting character
//******************************************************************
	uint8_t USART::putc_noblock(char data)
	{
		register uint8_t tmp_tx_last_byte = (tx_last_byte + 1) & TX_BUFFER_MASK;
		
		if(tx_first_byte == tmp_tx_last_byte)
			return BUFFER_FULL;
		
		tx_buffer[tmp_tx_last_byte] = data;
		tx_last_byte = tmp_tx_last_byte;
		
	#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
		switch(usartct)
		{
			default: // first found case as default (byte saving)
		#ifndef NO_TX0_INTERRUPT
			case 0:
			UCSR0B_REGISTER |= (1<<UDRIE0_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX0_INTERRUPT
		#ifndef NO_TX1_INTERRUPT
			case 1:
			UCSR1B_REGISTER |= (1<<UDRIE1_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX1_INTERRUPT
		#ifndef NO_TX2_INTERRUPT
			case 2:
			UCSR2B_REGISTER |= (1<<UDRIE2_BIT); // enable UDRE interrupt
			break;
		#endif // NO_TX2_INTERRUPT
		#ifndef NO_TX3_INTERRUPT
			case 3:
			UCSR3B_REGISTER |= (1<<UDRIE3_BIT); // enable UDRE interrupt
			//break;
		#endif // NO_TX3_INTERRUPT
		}
	#else
		UCSR0B_REGISTER |= (1<<UDRIE0_BIT); // enable UDRE interrupt
	#endif
		return COMPLETED;
	}

//******************************************************************
//Function  : Send string array.
//Arguments : Pointer to string array terminated by NULL.
//Return    : none
//******************************************************************
	void USART::putstr(char *string)
	{
		while(*string)
			this -> putc(*string++);
	}

//******************************************************************
//Function  : Send string not terminated by NULL or part of the string array.
//Arguments : 1. Pointer to string array.
//          : 2. Number of characters/bytes to send.
//Return    :    none
//******************************************************************
	void USART::putstr(char *string, uint8_t BytesToWrite)
	{
		while(BytesToWrite--)
			this -> putc(*string++);
	}

//******************************************************************
//Function  : Send string from flash memory.
//Arguments : Pointer to string placed in flash memory.
//Return    : none
//******************************************************************
	void USART::puts_p(const char *string)
	{
		register char c;
		while ((c = pgm_read_byte(string++)) ) this -> putc(c);
	}

//******************************************************************
//Function  : Send integer formated into ASCI string (base 10).
//Arguments : int16_t data value.
//Return    : none
//******************************************************************
	void USART::putint(int16_t data)
	{
		char buffer[7]; // heading, 5 digit bytes, NULL
		itoa(data, buffer, 10);

		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send integer formated into ASCI string.
//Arguments : 1. int16_t data value.
//          : 2. Base value (DEC, HEX, OCT, BIN, etc.).
//Return    :    none
//******************************************************************
	void USART::putint(int16_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		itoa(data, buffer, radix);

		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send unsigned integer formated into ASCI string (base 10).
//Arguments : uint16_t data value.
//Return    : none
//******************************************************************
	void USART::putuint(uint16_t data)
	{
		char buffer[7]; // heading, 5 digit bytes, NULL
		utoa(data, buffer, 10);

		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send unsigned integer formated into ASCI string.
//Arguments : 1. uint16_t data value.
//          : 2. Base value (DEC, HEX, OCT, BIN, etc.).
//Return    :    none
//******************************************************************
	void USART::putuint(uint16_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		utoa(data, buffer, radix);

		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send long integer formated into ASCI string (base 10).
//Arguments : int32_t data value.
//Return    : none
//******************************************************************
	void USART::putlong(int32_t data)
	{
		char buffer[12]; // heading, 10 digit bytes, NULL
		ltoa(data, buffer, 10);
		
		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send long integer formated into ASCI string.
//Arguments : 1. int32_t data value.
//          : 2. Base value (DEC, HEX, OCT, BIN, etc.).
//Return    :    none
//******************************************************************
	void USART::putlong(int32_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		ltoa(data, buffer, radix);
		
		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send unsigned long integer formated into ASCI string (base 10).
//Arguments : uint32_t data value.
//Return    : none
//******************************************************************
	void USART::putulong(uint32_t data)
	{
		char buffer[12]; // heading, 10 digit bytes, NULL
		ultoa(data, buffer, 10);
		
		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send unsigned long integer formated into ASCI string.
//Arguments : 1. uint32_t data value.
//          : 2. Base value (DEC, HEX, OCT, BIN, etc.).
//Return    :    none
//******************************************************************
	void USART::putulong(uint32_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		ultoa(data, buffer, radix);
		
		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send integer formated into ASCI string (base 16)
//Arguments : uint16_t data value.
//Return    : none
//******************************************************************
	void USART::put_hex(uint16_t data)
	{
		char buffer[6]; // heading, 4 digit bytes, NULL
		utoa(data, buffer, 16);
		
		this -> putstr(buffer);
	}

//******************************************************************
//Function  : Send floating point value formated into ASCI string.
//Arguments : float data value.
//Return    : none
//******************************************************************
	void USART::putfloat(float data)
	{
		char buffer[16];
		dtostrf(data, 15, 6, buffer);
		
		char *p = buffer;
		while(*p == ' ') // remove all unwanted spaces
			p++;
		
		this -> putstr(p);
	}
	
//******************************************************************
//Function  : Send floating point integer formated into ASCI string.
//Arguments : 1. Float data value.
//          : 2. Number of displayed digits after the dot.
//Return    :    none
//******************************************************************
	void USART::putfloat(float data, uint8_t precision)
	{
		char buffer[16];
		dtostrf(data, 15, precision, buffer);
		
		char *p = buffer;
		while(*p == ' ') // remove all unwanted spaces
			p++;
		
		this -> putstr(p);
	}
#endif // NO_USART_TX

#ifndef NO_USART_RX

//******************************************************************
//Function  : To receive single character/byte.
//Arguments : none
//Return    : Received character or NULL if buffer is empty.
//******************************************************************
	char USART::getc(void)
	{
		register uint8_t tmp_rx_first_byte = rx_first_byte;
		
		if(tmp_rx_first_byte == rx_last_byte) return 0;
		rx_first_byte = tmp_rx_first_byte = (tmp_rx_first_byte+1) & RX_BUFFER_MASK;
		
	#ifdef RX_GETC_ECHO
		register char tmp = rx_buffer[tmp_rx_first_byte];
		
	#ifdef RX_NEWLINE_MODE_N
		if(tmp == '\n') this -> putc('\r');
	#endif
		
		this -> putc(tmp);
		
	#ifdef RX_NEWLINE_MODE_R
		if(tmp == '\r') this -> putc('\n');
	#endif
	
		return tmp;
	#else
		return rx_buffer[tmp_rx_first_byte];
	#endif

	}

//******************************************************************
//Function  : Reads string from receiver buffer.
//Arguments : Pointer to array to fill with received string.
//Return    : none
//Note      : Received string will be terminated by NULL.
//          : DEPRECATED - possibility of buffer overflows
//******************************************************************
	void USART::gets(char *buffer)
	{
		do *buffer = this -> getc();
		while(*buffer++);
	}
	
//******************************************************************
//Function  : Reads string from receiver buffer
//Arguments : 1. Pointer to array to fill with received string.
//          : 2. Limit for receiving string size (array size)
//Return    :    none
//Note      : Received string will be terminated by NULL positioned at bufferlimit-1
//          : or at the end of the string if it's shorter than bufferlimit-1
//			: terminators CR LF will not be cut
//******************************************************************
	void USART::gets(char *buffer, uint8_t bufferlimit)
	{
		while(--bufferlimit)
		{
			*buffer = this -> getc();
			if(*buffer++ == 0)
				return;
		}
		*buffer = 0; // set last byte in buffer to NULL 
	}


//******************************************************************
//Function  : Reads one line from the receiver buffer. (waits for EOL terminator)
//Arguments : 1. Pointer to array to fill with received string.
//          : 2. Limit for receiving string size (array size)
//Return    :    none
//Note      : Received string will be terminated by NULL positioned at bufferlimit-1
//          : or at the end of the string if it's shorter than bufferlimit-1
//          : CR and LF terminators will be cut.
//          : Function will return if bufferlimit is reached without waiting for newline terminator
//******************************************************************
	void USART::getln(char *buffer, uint8_t bufferlimit)
	{
		while(--bufferlimit)
		{
			do{
				*buffer = this -> getc();
			}while(*buffer == 0);
			
		#ifdef RX_NEWLINE_MODE_N
			if(*buffer == '\n')
		#else
			if(*buffer == '\r')
		#endif
			{
			#ifdef RX_NEWLINE_MODE_RN
				while( !(this -> getc()) );
			#endif
				break;
			}
			buffer++;
		}
		*buffer = 0;
	}

//******************************************************************
//Function  : Reads burst of characters until first whitespace (waits for EOL terminator or first whitespace)
//Arguments : 1. Pointer to array to fill with received string.
//          : 2. Limit for receiving string size (array size)
//Return    :    none
//Note      : Received string will be terminated by NULL positioned at bufferlimit-1
//          : or at the end of the string if it's shorter than bufferlimit-1
//          : CR and LF terminators will be cut.
//          : Function will return if bufferlimit is reached without waiting for newline terminator
//          : Function will cut all whitespaces before first nonspace character
//******************************************************************
	void USART::getlnToFirstWhiteSpace(char *buffer, uint8_t bufferlimit)
	{
		*buffer++ = this -> skipWhiteSpaces();
		bufferlimit--;
		
		while(--bufferlimit)
		{
			do{
				*buffer = this -> getc();
			} while(*buffer == 0);
			
		#ifdef RX_NEWLINE_MODE_N
			if(*buffer == '\n')
		#else // RX_NEWLINE_MODE_R
			if(*buffer == '\r')
		#endif
			{
			#ifdef RX_NEWLINE_MODE_RN
				while( !(this -> getc()) );
			#endif
				break;
			}
			else if(*buffer <= 32)
				break; // string reading is done, we will exit

			buffer++;
		}
		*buffer = 0;
	}

//******************************************************************
//Function  : To skip all incoming whitespace characters until first nonspace character.
//Arguments : none
//Return    : First received nonspace character.
//Note      : First nonspace character is cut from receiver buffer.
//******************************************************************
	char USART::skipWhiteSpaces(void)
	{
		register char c;
		
		do{
			c = this -> getc();
		}while(c <= 32);
		
		return c;
	}
//******************************************************************
//Function  : Read 16bit integer value from the input stream.
//Arguments : none
//Return    : Received 16bit integer value.
//******************************************************************
	int16_t USART::getint(void)
	{
		char buff[32];
		this -> getlnToFirstWhiteSpace(buff, 32);
		
		return atoi(buff);
	}

//******************************************************************
//Function  : Read 32bit integer value from the input stream.
//Arguments : none
//Return    : Received 32bit integer value
//******************************************************************
	int32_t USART::getlong(void)
	{
		char buff[32];
		this -> getlnToFirstWhiteSpace(buff, 32);
		
		return atol(buff);
	}
	
//******************************************************************
//Function  : Read floating point value from the input stream.
//Arguments : none
//Return    : Received float value.
//******************************************************************
	float USART::getfloat(void)
	{
		char buff[32];
		this -> getlnToFirstWhiteSpace(buff, 32);
		
		return atof(buff);
	}
	
//******************************************************************
//Function  : To receive single byte in binary transmission.
//Arguments : Pointer to byte which have to be filed by incoming data.
//Return    : Status value: 0 = BUFFER_EMPTY, 1 = COMPLETED.
//Note      : This function doesn't cut CR, LF, NULL terminators
//          : If receiver buffer is empty return status = BUFFER_EMPTY instead of returning NULL (as in getc).
//******************************************************************
	uint8_t USART::getData(uint8_t *data)
	{
		register uint8_t tmp_rx_first_byte = rx_first_byte;
		
		if(tmp_rx_first_byte == rx_last_byte) return BUFFER_EMPTY; // result = 0
		
		rx_first_byte = tmp_rx_first_byte = (tmp_rx_first_byte+1) & RX_BUFFER_MASK;
		*data = rx_buffer[tmp_rx_first_byte];
		
		return COMPLETED; // result = 1
	}

//******************************************************************
//Function  : To check how many bytes are waiting in the receiver buffer.
//Arguments : none
//Return    : Number of bytes waiting in receiver buffer.
//******************************************************************
	uint8_t USART::AvailableBytes(void)
	{
		return (rx_last_byte - rx_first_byte) & RX_BUFFER_MASK;
	}
	
#endif // NO_USART_RX

#ifndef NO_TX0_INTERRUPT
	ISR(TX0_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART0 -> tx_first_byte;
		
		if(tmp_tx_first_byte != pUSART0 -> tx_last_byte)
		{
			tmp_tx_first_byte = (tmp_tx_first_byte + 1) & TX0_BUFFER_MASK; // calculate new position of TX head in buffer
			
			UDR0_REGISTER = pUSART0 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
			pUSART0 -> tx_first_byte = tmp_tx_first_byte;
		}
		else
		{
			UCSR0B_REGISTER &= ~(1<<UDRIE0_BIT);
		}
		
	}
#endif // NO_TX0_INTERRUPT

#ifndef NO_RX0_INTERRUPT
	ISR(RX0_INTERRUPT)
	{
		register uint8_t tmp_rx_last_byte = (pUSART0 -> rx_last_byte + 1) & RX0_BUFFER_MASK;
		register uint8_t tmp = UDR0_REGISTER; // save received character to temporary register
		
		if(pUSART0 -> rx_first_byte != tmp_rx_last_byte)
		{
			pUSART0 -> rx_buffer[tmp_rx_last_byte] = tmp;
			pUSART0 -> rx_last_byte = tmp_rx_last_byte;
		}
		
	}
#endif // NO_RX0_INTERRUPT

#ifndef NO_TX1_INTERRUPT
	ISR(TX1_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART1 -> tx_first_byte;
		
		if(tmp_tx_first_byte != pUSART1 -> tx_last_byte)
		{
			tmp_tx_first_byte = (tmp_tx_first_byte + 1) & TX1_BUFFER_MASK; // calculate new position of TX head in buffer
			
			UDR1_REGISTER = pUSART1 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
			pUSART1 -> tx_first_byte = tmp_tx_first_byte;
		}
		else
		{
			UCSR1B_REGISTER &= ~(1<<UDRIE1_BIT);
		}
		
	}
#endif // NO_TX1_INTERRUPT

#ifndef NO_RX1_INTERRUPT
	ISR(RX1_INTERRUPT)
	{
		register uint8_t tmp_rx_last_byte = (pUSART1 -> rx_last_byte + 1) & RX1_BUFFER_MASK;
		register uint8_t tmp = UDR1_REGISTER; // save received character to temporary register
		
		if(pUSART1 -> rx_first_byte != tmp_rx_last_byte)
		{
			pUSART1 -> rx_buffer[tmp_rx_last_byte] = tmp;
			pUSART1 -> rx_last_byte = tmp_rx_last_byte;
		}
		
	}
#endif // NO_RX1_INTERRUPT

#ifndef NO_TX2_INTERRUPT
	ISR(TX2_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART2 -> tx_first_byte;
		
		if(tmp_tx_first_byte != pUSART2 -> tx_last_byte)
		{
			tmp_tx_first_byte = (tmp_tx_first_byte + 1) & TX2_BUFFER_MASK; // calculate new position of TX head in buffer
			
			UDR2_REGISTER = pUSART2 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
			pUSART2 -> tx_first_byte = tmp_tx_first_byte;
		}
		else
		{
			UCSR2B_REGISTER &= ~(1<<UDRIE2_BIT);
		}
	
	}
#endif // NO_TX2_INTERRUPT

#ifndef NO_RX2_INTERRUPT
	ISR(RX2_INTERRUPT)
	{
		register uint8_t tmp_rx_last_byte = (pUSART2 -> rx_last_byte + 1) & RX2_BUFFER_MASK;
		register uint8_t tmp = UDR2_REGISTER; // save received character to temporary register
		
		if(pUSART2 -> rx_first_byte != tmp_rx_last_byte)
		{
			pUSART2 -> rx_buffer[tmp_rx_last_byte] = tmp;
			pUSART2 -> rx_last_byte = tmp_rx_last_byte;
		}
	
	}
#endif // NO_RX2_INTERRUPT

#ifndef NO_TX3_INTERRUPT
	ISR(TX3_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART3 -> tx_first_byte;
				
		if(tmp_tx_first_byte != pUSART3 -> tx_last_byte)
		{
			tmp_tx_first_byte = (tmp_tx_first_byte + 1) & TX3_BUFFER_MASK; // calculate new position of TX head in buffer
					
			UDR3_REGISTER = pUSART3 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
			pUSART3 -> tx_first_byte = tmp_tx_first_byte;
		}
		else
		{
			UCSR3B_REGISTER &= ~(1<<UDRIE3_BIT);
		}
		
	}
#endif // NO_TX3_INTERRUPT

#ifndef NO_RX3_INTERRUPT
	ISR(RX3_INTERRUPT)
	{
		register uint8_t tmp_rx_last_byte = (pUSART3 -> rx_last_byte + 1) & RX3_BUFFER_MASK;
		register uint8_t tmp = UDR3_REGISTER; // save received character to temporary register
	
		if(pUSART3 -> rx_first_byte != tmp_rx_last_byte) 
		{
			pUSART3 -> rx_buffer[tmp_rx_last_byte] = tmp;
			pUSART3 -> rx_last_byte = tmp_rx_last_byte;
		}
		
	}
#endif // NO_RX3_INTERRUPT



