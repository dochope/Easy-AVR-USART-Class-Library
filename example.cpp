#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usart.hpp"


int main(void)
{
	USART uart0, uart1(BAUD_CALC(115200), 1), serial(BAUD_CALC(19200), 3); // usart0 is set to 9600 baud and 8n1 transmission as default
	sei(); // enable interrupts, library wouldn't work without this
	
	uart0.puts("hello from usart 0\r\n"); // write const string to usart buffer 
	// if you do not have enough SRAM memory space to keep all strings, try to use puts_P instead
	uart1.puts_P("hello from flashed, usart 1\r\n"); // write string to usart buffer from flash memory
	
	serial.puts("hello from quad uarted serial (usart3)\r\n");
	serial.putfloat(0.1337f);
	
	char buffer[13];
	
	while(1)
    {
		uart0.puts("bytes waiting in buffer : ");
		uart0.putint(uart0.AvailableBytes()); // ask for bytes waiting in receiver buffer
		uart0.gets(buffer, 13); // read 13 bytes or one line from usart buffer
		uart0.puts("/r/n");
		
		uart1.putstr(buffer); // write dynamic string to usart buffer
		_delay_ms(5000);
    }
}