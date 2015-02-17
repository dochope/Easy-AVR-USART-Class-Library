#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usart.hpp"


int main(void)
{
	USART uart0, uart1(BAUD_CALC(115200), 1); // usart0 is set to 9600 baud and 8n1 transmission as default
	sei(); // enable interrupts, library wouldn't work without this
	
	uart0.puts("hello from usart 0\r\n"); // write const string to uart buffer 
	//if you dont have enough memory space to keep all strings, try to use puts_P
	uart1.puts_P("hello from flashed, usart 1\r\n"); // write string to uart buffer from flash memory
	
	char buffer[13];
	
	while(1)
    {
		uart0.gets(buffer, 13); // read 13 bytes or one line from uart buffer
		uart1.putstr(buffer); // write dynamic string to uart buffer
		_delay_ms(1000);
    }
}