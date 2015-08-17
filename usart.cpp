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
	
	void USART::set_UCSRC(uint8_t UCSRC_reg);
	{
		switch(usartct)
		{
			default:
		#ifdef USE_USART0
			case 0: UCSR0C_REGISTER |= UCSRC_reg; break;
		#endif // USE_USART0
		#ifdef USE_USART1
			case 1: UCSR1C_REGISTER |= UCSRC_reg; break;
		#endif // USE_USART1
		#ifdef USE_USART2
			case 2: UCSR2C_REGISTER |= UCSRC_reg; break;
		#endif // USE_USART2
		#ifdef USE_USART3
			case 3: UCSR3C_REGISTER |= UCSRC_reg; //break;
		#endif // USE_USART3
		}
		
	}
	
	void USART::set_U2X(void) 
	{
		switch(usartct)
		{
			default:
		#ifdef USE_USART0
			case 0: UCSR0A_REGISTER |= (1<<U2X0_BIT); break;
		#endif // USE_USART0
		#ifdef USE_USART1
			case 1: UCSR1A_REGISTER |= (1<<U2X1_BIT); break;
		#endif // USE_USART1
		#ifdef USE_USART2
			case 2: UCSR2A_REGISTER |= (1<<U2X2_BIT); break;
		#endif // USE_USART2
		#ifdef USE_USART3
			case 3: UCSR3A_REGISTER |= (1<<U2X3_BIT); //break;
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
	
	void USART::set_UCSRC(uint8_t UCSRC_reg)
	{
		UCSR0C_REGISTER |= UCSRC_reg;
	}
	
	void USART::set_U2X(void)
	{
		UCSR0A_REGISTER |= (1<<U2X0_BIT);
	}
#endif //USE_USART1

#ifndef NO_USART_TX
	void USART::putc(char data)
	{
		register uint8_t tmp_tx_last_byte = (tx_last_byte + 1) & TX_BUFFER_MASK; ;
		
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

	void USART::putstr(char *string)
	{
		while(*string)
			this -> putc(*string++);
	}
	
	void USART::putstr(char *string, uint8_t BytesToWrite)
	{
		while(BytesToWrite--)
			this -> putc(*string++);
	}

	void USART::puts_p(const char *string)
	{
		register char c;
		while ((c = pgm_read_byte(string++)) ) this -> putc(c);
	}
	
	void USART::putint(int16_t data)
	{
		char buffer[7]; // heading, 5 digit bytes, NULL
		itoa(data, buffer, 10);

		this -> putstr(buffer);
	}
	
	void USART::putint(int16_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		itoa(data, buffer, radix);

		this -> putstr(buffer);
	}
	
	void USART::putint(uint16_t data)
	{
		char buffer[7]; // heading, 5 digit bytes, NULL
		utoa(data, buffer, 10);

		this -> putstr(buffer);
	}
		
	void USART::putint(uint16_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		utoa(data, buffer, radix);

		this -> putstr(buffer);
	}
	
	void USART::putlong(int32_t data)
	{
		char buffer[12]; // heading, 10 digit bytes, NULL
		ltoa(data, buffer, 10);
		
		this -> putstr(buffer);
	}
	
	void USART::putlong(int32_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		ltoa(data, buffer, radix);
		
		this -> putstr(buffer);
	}
	
	void USART::putlong(uint32_t data)
	{
		char buffer[12]; // heading, 10 digit bytes, NULL
		ultoa(data, buffer, 10);
		
		this -> putstr(buffer);
	}
	
	void USART::putlong(uint32_t data, uint8_t radix)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		ultoa(data, buffer, radix);
		
		this -> putstr(buffer);
	}
	
	void USART::put_hex(int16_t data)
	{
		char buffer[6]; // heading, 4 digit bytes, NULL
		utoa(data, buffer, 16);
		
		this -> putstr(buffer);
	}
	
	void USART::putfloat(float data)
	{
		char buffer[16];
		dtostrf(data, 15, 6, buffer);
		
		char *p = buffer;
		while(*p == ' ') // remove all unwanted spaces
			p++;
		
		this -> putstr(p);
	}
	
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

	void USART::gets(char *buffer)
	{
		do *buffer = this -> getc();
		while(*buffer++);
	}
	
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
	
	char USART::skipWhiteSpaces(void)
	{
		register char c;
		
		do{
			c = this -> getc();
		}while(c <= 32);
		
		return c;
	}
	
	uint8_t USART::getData(uint8_t *data)
	{
		register uint8_t tmp_rx_first_byte = rx_first_byte;
		
		if(tmp_rx_first_byte == rx_last_byte) return BUFFER_EMPTY; // result = 0
		
		rx_first_byte = tmp_rx_first_byte = (tmp_rx_first_byte+1) & RX_BUFFER_MASK;
		*data = rx_buffer[tmp_rx_first_byte];
		
		return COMPLETED; // result = 1
	}
	
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



