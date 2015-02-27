#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.hpp"

	USART *pUSART0;

#ifdef USE_USART1
	USART *pUSART1;
#endif

#ifdef USE_USART1
	USART::USART(uint16_t baudRate, uint8_t usartcnter)
	{
		usartct = usartcnter;
		if(usartcnter == 0)
		{
			UBRR0L_REGISTER = (uint8_t) baudRate;
			UBRR0H_REGISTER = (baudRate>>8);
		
		#ifdef USART0_U2X_SPEED
			UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
		#endif
			UCSR0B_REGISTER = USART0_CONFIG_B; 
			// (1<<TXCIE0_BIT)|(1<<RXCIE0_BIT)|(1<<TXEN0_BIT)|(1<<RXEN0_BIT); 
			// 8n1 is set by default, setting UCSRC is not needed
			pUSART0 = this; // IS SPARTA
		}
		else
		{
			UBRR1L_REGISTER = (uint8_t) baudRate;
			UBRR1H_REGISTER = (baudRate>>8);
			
		#ifdef USART1_U2X_SPEED
			UCSR1A_REGISTER |= (1<<U2X1_BIT); // enable double speed
		#endif
			UCSR1B_REGISTER = USART1_CONFIG_B; 
			// (1<<TXCIE1_BIT)|(1<<RXCIE1_BIT)|(1<<TXEN1_BIT)|(1<<RXEN1_BIT); 
			// 8n1 is set by default, setting UCSRC is not needed
			pUSART1 = this; // IS SPARTA
		}
		
	#ifndef NO_USART_TX
		interrupt_semaphore = unlocked;
	#endif
	}
	
	USART::USART(uint8_t UCSRC_reg, uint16_t baudRate, uint8_t usartcnter)
	{
		usartct = usartcnter;
		if(usartcnter == 0)
		{
			UBRR0L_REGISTER = (uint8_t) baudRate;
			UBRR0H_REGISTER = (baudRate>>8);
			
		#ifdef USART0_U2X_SPEED
			UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
		#endif
			UCSR0B_REGISTER = USART0_CONFIG_B; 
			// (1<<TXCIE0_BIT)|(1<<RXCIE0_BIT)|(1<<TXEN0_BIT)|(1<<RXEN0_BIT);
			UCSR0C_REGISTER = UCSRC_reg;
			
			pUSART0 = this; // IS SPARTA
		}
		else
		{
			UBRR1L_REGISTER = (uint8_t) baudRate;
			UBRR1H_REGISTER = (baudRate>>8);
			
		#ifdef USART1_U2X_SPEED
			UCSR1A_REGISTER |= (1<<U2X1_BIT); // enable double speed
		#endif
			UCSR1B_REGISTER = USART1_CONFIG_B; 
			// (1<<TXCIE1_BIT)|(1<<RXCIE1_BIT)|(1<<TXEN1_BIT)|(1<<RXEN1_BIT);
			UCSR1C_REGISTER = UCSRC_reg;
			
			pUSART1 = this; // IS SPARTA
		}
		
	#ifndef NO_USART_TX
		interrupt_semaphore = unlocked;
	#endif
	}
	
#else // no USART1
	
	USART::USART(uint16_t baudRate)
	{
		UBRR0L_REGISTER = (uint8_t) baudRate;
		UBRR0H_REGISTER = (baudRate>>8);
	
	#ifdef USART0_U2X_SPEED
		UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
	#endif
		UCSR0B_REGISTER = USART0_CONFIG_B; 
		// (1<<TXEN0_BIT)|(1<<RXEN0_BIT)|(1<<TXCIE0_BIT)|(1<<RXCIE0_BIT); 
		// 8n1 is set by default, setting UCSRC is not needed 
		pUSART0 = this; // IS SPARTA
	
	#ifndef NO_USART_TX
		interrupt_semaphore = unlocked;
	#endif
	}
	
	USART::USART(uint8_t UCSRC_reg, uint16_t baudRate)
	{
		UBRR0L_REGISTER = (uint8_t) baudRate;
		UBRR0H_REGISTER = (baudRate>>8);
		
	#ifdef USART0_U2X_SPEED
		UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
	#endif
		UCSR0B_REGISTER = USART0_CONFIG_B; 
		// (1<<TXEN0_BIT)|(1<<RXEN0_BIT)|(1<<TXCIE0_BIT)|(1<<RXCIE0_BIT);
		UCSR0C_REGISTER = UCSRC_reg;
		
		pUSART0 = this; // IS SPARTA
		
	#ifndef NO_USART_TX
		interrupt_semaphore = unlocked;
	#endif
	}
#endif //USE_USART1

#ifndef NO_USART_TX
	void USART::putc(char data)
	{
		register uint8_t tmp_tx_last_byte = tx_last_byte; // saves 2 bytes
		register uint8_t tmp_tx_first_byte = tx_first_byte; // saves 2 bytes
		
		tx_buffer[tmp_tx_last_byte] = data;
		tx_last_byte = (tmp_tx_last_byte + 1) & TX_BUFFER_MASK; // calculate new position of TX tail in buffer
		
		while(tmp_tx_first_byte == tx_last_byte); // wait for free space in buffer
		
			if(interrupt_semaphore == unlocked) // if transmitter interrupt is disabled
			{
				interrupt_semaphore = locked;
				
			#ifdef USE_USART1
				if(usartct == 0)
					UDR0_REGISTER = tx_buffer[tmp_tx_first_byte]; // enable transmitter interrupt
				else
					UDR1_REGISTER = tx_buffer[tmp_tx_first_byte]; // enable transmitter interrupt
			#else
					UDR0_REGISTER = tx_buffer[tmp_tx_first_byte]; // enable transmitter interrupt
			#endif
			}
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
		while(pgm_read_byte(string)) 
			this -> putc(pgm_read_byte(string++));
	}
	
	void USART::putint(int16_t data)
	{
		char buffer[7]; // heading, 5 digit bytes, NULL
		itoa(data, buffer, 10);

		this -> putstr(buffer);
	}
	
	void USART::put_hex(int16_t data)
	{
		char buffer[6]; // heading, 4 digit bytes, NULL
		itoa(data, buffer, 16);
		
		this -> putstr(buffer);
	}
	
	void USART::putlong(int32_t data)
	{
		char buffer[17]; // heading, 15 digit bytes, NULL
		ltoa(data, buffer, 10);
		
		this -> putstr(buffer);
	}
#endif // NO_USART_TX

#ifndef NO_USART_RX
	char USART::getc(void)
	{
		register char temp;
		
		register uint8_t tmp_rx_first_byte = rx_first_byte; // saves 4 bytes
		register uint8_t tmp_rx_last_byte = rx_last_byte; // saves 8 bytes // no one knows how
		
		temp = (tmp_rx_first_byte == tmp_rx_last_byte) ? 0:rx_buffer[tmp_rx_first_byte]; 
		if(tmp_rx_first_byte != tmp_rx_last_byte) 
			rx_first_byte = (tmp_rx_first_byte+1) & RX_BUFFER_MASK; // calculate new position of RX head in buffer
		
		if(temp == '\n')   temp = 0;
		return temp;
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
		*buffer = 0;
	}
	
	uint8_t USART::getbin(uint8_t &data)
	{
		register uint8_t tmp_rx_first_byte = rx_first_byte;
		
		data = rx_buffer[tmp_rx_first_byte];
		if(tmp_rx_first_byte != rx_last_byte) // if buffer is not empty
		{
			rx_first_byte = (tmp_rx_first_byte+1) & RX_BUFFER_MASK; // calculate new position of RX head in buffer
			return COMPLETED; // result = 0
		}
		else 
			return BUFFER_EMPTY; // in this case data value is a trash // result = 1
	}
#endif // NO_USART_RX

#ifndef NO_TX0_INTERRUPT
	ISR(TX0_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART0 -> tx_first_byte = (pUSART0 -> tx_first_byte + 1) & TX_BUFFER_MASK; 
		// calculate new position of TX head in buffer, write back and use it as register variable // saved 4 bytes
		
		if(tmp_tx_first_byte != pUSART0 -> tx_last_byte)
		{
			UDR0_REGISTER = pUSART0 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
		}
		else
		{
			pUSART0 -> interrupt_semaphore = unlocked;
		}
	}
#endif // NO_TX0_INTERRUPT

#ifndef NO_RX0_INTERRUPT
	ISR(RX0_INTERRUPT)
	{
		register char tmp;
		tmp = UDR0_REGISTER; // save received character to temporary register
	
		register uint8_t tmp_rx_last_byte = pUSART0 -> rx_last_byte + 1; // saves 20 bytes // working only in this way, in the other adds 18 bytes to stock size
		
	#ifdef RX0_BINARY_MODE // not optimal and no one knows why 
		if(pUSART0 -> rx_first_byte != (tmp_rx_last_byte)) // tmp_rx_last_byte + 1 
	#else
		if(pUSART0 -> rx_first_byte != (tmp_rx_last_byte) && (tmp != '\r')) // tmp_rx_last_byte + 1 
	#endif
		{
			pUSART0 -> rx_buffer[tmp_rx_last_byte-1] = tmp; // tmp_rx_last_byte 
			pUSART0 -> rx_last_byte = (tmp_rx_last_byte) & RX_BUFFER_MASK; // calculate new position of RX tail in buffer // tmp_rx_last_byte + 1
		}
	}
#endif // NO_RX0_INTERRUPT

#ifdef USE_USART1

#ifndef NO_TX1_INTERRUPT
	ISR(TX1_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART1 -> tx_first_byte = (pUSART1 -> tx_first_byte + 1) & TX_BUFFER_MASK;
	
		if(tmp_tx_first_byte != pUSART1 -> tx_last_byte)
		{
			UDR1_REGISTER = pUSART1 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
		}
		else
		{
			pUSART1 -> interrupt_semaphore = unlocked;
		}
	}
#endif // NO_TX1_INTERRUPT

#ifndef NO_RX1_INTERRUPT
	ISR(RX1_INTERRUPT)
	{
		register char tmp;
		tmp = UDR1_REGISTER; // save received character to temporary register
	
		register uint8_t tmp_rx_last_byte = pUSART1 -> rx_last_byte + 1;
		
	#ifdef RX1_BINARY_MODE
		if(pUSART1 -> rx_first_byte != (tmp_rx_last_byte)) // pUSART1 -> rx_last_byte+1
	#else
		if(pUSART1 -> rx_first_byte != (tmp_rx_last_byte) && (tmp != '\r'))	// pUSART1 -> rx_last_byte+1
	#endif
		{
			pUSART1 -> rx_buffer[tmp_rx_last_byte-1] = tmp;	// pUSART1 -> rx_last_byte
			pUSART1 -> rx_last_byte = (tmp_rx_last_byte) & RX_BUFFER_MASK;	// pUSART1 -> rx_last_byte+1
		}
	}
#endif // NO_RX1_INTERRUPT

#endif // USE_USART1