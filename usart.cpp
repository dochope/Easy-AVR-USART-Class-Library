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
	
	USART::USART(uint16_t baudRate, uint8_t usartcnter)
	{
		usartct = usartcnter;
		switch(usartcnter)
		{
		#ifdef USE_USART1
			case 0:
				UBRR0L_REGISTER = (uint8_t) baudRate;
				UBRR0H_REGISTER = (baudRate>>8);
		
			#ifdef USART0_U2X_SPEED
				UCSR0A_REGISTER |= (1<<U2X0_BIT); // enable double speed
			#endif
				UCSR0B_REGISTER = USART0_CONFIG_B; 
				// (1<<TXCIE0_BIT)|(1<<RXCIE0_BIT)|(1<<TXEN0_BIT)|(1<<RXEN0_BIT); 
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART0 = this; // IS SPARTA
				break;
		#endif // USE_USART0
		#ifdef USE_USART1
			case 1:
				UBRR1L_REGISTER = (uint8_t) baudRate;
				UBRR1H_REGISTER = (baudRate>>8);
		
			#ifdef USART1_U2X_SPEED
				UCSR1A_REGISTER |= (1<<U2X1_BIT); // enable double speed
			#endif
				UCSR1B_REGISTER = USART1_CONFIG_B;
				// (1<<TXCIE1_BIT)|(1<<RXCIE1_BIT)|(1<<TXEN1_BIT)|(1<<RXEN1_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART1 = this; // IS SPARTA
				break;
			
		#endif // USE_USART1
		#ifdef USE_USART2
			case 2:
				UBRR2L_REGISTER = (uint8_t) baudRate;
				UBRR2H_REGISTER = (baudRate>>8);
		
			#ifdef USART2_U2X_SPEED
				UCSR2A_REGISTER |= (1<<U2X2_BIT); // enable double speed
			#endif
				UCSR2B_REGISTER = USART2_CONFIG_B;
				// (1<<TXCIE2_BIT)|(1<<RXCIE2_BIT)|(1<<TXEN2_BIT)|(1<<RXEN2_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART2 = this; // IS SPARTA
				break;
		
		#endif // USE_USART2
		#ifdef USE_USART3
			case 3:
				UBRR3L_REGISTER = (uint8_t) baudRate;
				UBRR3H_REGISTER = (baudRate>>8);
		
			#ifdef USART3_U2X_SPEED
				UCSR3A_REGISTER |= (1<<U2X3_BIT); // enable double speed
			#endif
				UCSR3B_REGISTER = USART3_CONFIG_B;
				// (1<<TXCIE3_BIT)|(1<<RXCIE3_BIT)|(1<<TXEN3_BIT)|(1<<RXEN3_BIT);
				// 8n1 is set by default, setting UCSRC is not needed
				pUSART3 = this; // IS SPARTA
				break;
		#endif // USE_USART3
		}
		
	#ifndef NO_USART_TX
		interrupt_semaphore = unlocked;
	#endif
	}
	
	void USART::set_UCSRC(uint8_t UCSRC_reg);
	{
		switch(usartct)
		{
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
		register uint8_t tmp_tx_last_byte = tx_last_byte;
		//register uint8_t tmp_tx_first_byte = tx_first_byte; // causes issues, makes infinite loop at transmit buffer overflow
		
		tx_buffer[tmp_tx_last_byte] = data;
		tmp_tx_last_byte = tx_last_byte = (tmp_tx_last_byte + 1) & TX_BUFFER_MASK; // calculate new position of TX tail in buffer
		
		while(tx_first_byte == tmp_tx_last_byte); // wait for free space in buffer
		
		if(interrupt_semaphore == unlocked) // if transmitter interrupt is disabled
		{
			interrupt_semaphore = locked;
				
		#if defined(USE_USART1)||defined(USE_USART2)||defined(USE_USART3)
			switch(usartct)
			{
			#ifndef NO_TX0_INTERRUPT
				case 0: UDR0_REGISTER = tx_buffer[tx_first_byte]; break; // enable transmitter interrupt
			#endif
			#ifndef NO_TX1_INTERRUPT
				case 1: UDR1_REGISTER = tx_buffer[tx_first_byte]; break; // enable transmitter interrupt
			#endif
			#ifndef NO_TX2_INTERRUPT
				case 2: UDR2_REGISTER = tx_buffer[tx_first_byte]; break; // enable transmitter interrupt
			#endif
			#ifndef NO_TX3_INTERRUPT
				case 3: UDR3_REGISTER = tx_buffer[tx_first_byte]; break; // enable transmitter interrupt
			#endif
			}
		#else
			UDR0_REGISTER = tx_buffer[tx_first_byte]; // enable transmitter interrupt
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
	void USART::putfloat(float data)
	{
		char buffer[16];
		dtostrf(data, 15, 6, buffer);
		
		char *p = buffer;
		while(*p == ' ') // remove all unwanted spaces
			p++;
		
		this -> putstr(p);
	}
	void USART::putfloat(float data, uint8_t size, uint8_t precision)
	{
		char buffer[size+1];
		dtostrf(data, size, precision, buffer);
		
		char *p = buffer;
		while(*p == ' ') // remove all unwanted spaces
			p++;
		
		this -> putstr(p);
	}
#endif // NO_USART_TX

#ifndef NO_USART_RX
	char USART::getc(void)
	{
		register char temp;
		
		register uint8_t tmp_rx_first_byte = rx_first_byte; 
		register uint8_t tmp_rx_last_byte = rx_last_byte;
		
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
		*buffer = 0; // set last byte in buffer to NULL 
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
	
	uint8_t USART::AvailableBytes(void)
	{
		return (rx_last_byte - rx_first_byte) & RX_BUFFER_MASK;
	}
#endif // NO_USART_RX

#ifndef NO_TX0_INTERRUPT
	ISR(TX0_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART0 -> tx_first_byte = (pUSART0 -> tx_first_byte + 1) & TX_BUFFER_MASK; 
		// calculate new position of TX head in buffer, write back and use it as register variable 
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

#ifndef NO_TX2_INTERRUPT
	ISR(TX2_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART2 -> tx_first_byte = (pUSART2 -> tx_first_byte + 1) & TX_BUFFER_MASK;
	
		if(tmp_tx_first_byte != pUSART2 -> tx_last_byte)
		{
			UDR2_REGISTER = pUSART2 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
		}
		else
		{
			pUSART2 -> interrupt_semaphore = unlocked;
		}
	
	}
#endif // NO_TX2_INTERRUPT

#ifndef NO_RX2_INTERRUPT
	ISR(RX2_INTERRUPT)
	{
		register char tmp;
		tmp = UDR2_REGISTER; // save received character to temporary register
	
		register uint8_t tmp_rx_last_byte = pUSART2 -> rx_last_byte + 1;
	
	#ifdef RX2_BINARY_MODE
		if(pUSART2 -> rx_first_byte != (tmp_rx_last_byte)) // pUSART2 -> rx_last_byte+1
	#else
		if(pUSART2 -> rx_first_byte != (tmp_rx_last_byte) && (tmp != '\r'))	// pUSART2 -> rx_last_byte+1
	#endif
		{
			pUSART2 -> rx_buffer[tmp_rx_last_byte-1] = tmp;	// pUSART2 -> rx_last_byte
			pUSART2 -> rx_last_byte = (tmp_rx_last_byte) & RX_BUFFER_MASK;	// pUSART2 -> rx_last_byte+1
		}
	
	}
#endif // NO_RX2_INTERRUPT

#ifndef NO_TX3_INTERRUPT
	ISR(TX3_INTERRUPT)
	{
		register uint8_t tmp_tx_first_byte = pUSART3 -> tx_first_byte = (pUSART3 -> tx_first_byte + 1) & TX_BUFFER_MASK;
	
		if(tmp_tx_first_byte != pUSART3 -> tx_last_byte)
		{
			UDR3_REGISTER = pUSART3 -> tx_buffer[tmp_tx_first_byte]; // transmit character from the buffer
		}
		else
		{
			pUSART3 -> interrupt_semaphore = unlocked;
		}
		
	}
#endif // NO_TX3_INTERRUPT

#ifndef NO_RX3_INTERRUPT
	ISR(RX3_INTERRUPT)
	{
		register char tmp;
		tmp = UDR3_REGISTER; // save received character to temporary register
	
		register uint8_t tmp_rx_last_byte = pUSART3 -> rx_last_byte + 1;
	
	#ifdef RX3_BINARY_MODE
		if(pUSART3 -> rx_first_byte != (tmp_rx_last_byte)) // pUSART3 -> rx_last_byte+1
	#else
		if(pUSART3 -> rx_first_byte != (tmp_rx_last_byte) && (tmp != '\r'))	// pUSART3 -> rx_last_byte+1
	#endif
		{
			pUSART3 -> rx_buffer[tmp_rx_last_byte-1] = tmp;	// pUSART3 -> rx_last_byte
			pUSART3 -> rx_last_byte = (tmp_rx_last_byte) & RX_BUFFER_MASK;	// pUSART3 -> rx_last_byte+1
		}
		
	}
#endif // NO_RX3_INTERRUPT



